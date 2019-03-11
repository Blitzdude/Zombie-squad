/*
	OneLoneCoder_PGE_ShadowCasting2D.cpp

	What is this?
	~~~~~~~~~~~~~
	This is an implementation of two algorithms that work together
	to produuce a "line-of-sight" or "shadow casting" effect depending
	upon your perspective. To use it compile with olcPixelGameEngine.h,
	place/remove blocks with left click and hold down right mouse button
	to illuminate the scene from the mouse cursor.

	The first algorithm converts a tile map into a set of geometric
	primitives whcih are more conveninet to use in geometric programs
	such as this. The second algorithm casts rays to create a triangle
	fan that represents the visible area form the source.

	Thanks to these resources for ideas
	https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	https://www.redblobgames.com/articles/visibility/
	https://ncase.me/sight-and-light/


	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Relevant Videos
	~~~~~~~~~~~~~~~
	https://youtu.be/kRH6oJLFYxY Introducing olcPixelGameEngine
	--> https://youtu.be/fc3nnG2CG8U Video about this very code!

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/
#include <iostream>
#include <algorithm>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


// TODO: World class
/*
	List of cells
	List of Edges
	GetCell()
	Clear()
	CreatePolymap()
	
*/

struct sIntersectResult
{
	sIntersectResult()
	{
		px = 0.0f; 
		py = 0.0f;
		t = -1.0f;
	};

	float px, py;
	float t;
};

struct vec2d
{
	float x, y;
};

// TODO: Add normals to this?, so if we a ball is overlapping, we can push it away
struct sEdge
{
	vec2d start;
	vec2d end;
};

struct sCell
{
	int edge_id[4];
	bool edge_exist[4];
	bool exist = false;
};

struct sEnemy // enemies are circles
{
	float x, y;
	bool visible;
	int count; // Number of intersections
};

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

class ShadowCasting2D : public olc::PixelGameEngine
{
public:
	ShadowCasting2D()
	{
		sAppName = "ShadowCasting2D";
	}

private:
	sCell* world;
	int nWorldWidth = 40;
	int nWorldHeight = 30;

	olc::Sprite *sprLightCast;
	olc::Sprite *buffLightRay;
	olc::Sprite *buffLightTex;

	vector<sEdge> vecEdges;
	vector<sEnemy> vecEnemies;


	//			angle	x	   y
	vector<tuple<float, float, float>> vecVisibilityPolygonPoints;

	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch)
	{
		// Clear "PolyMap"
		vecEdges.clear();

		// Clear each cells information
		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++)
				for (int j = 0; j < 4; j++)
				{
					world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
					world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
				}

		// Iterate through region from top left to bottom right
		for (int x = 1; x < w - 1; x++)
			for (int y = 1; y < h - 1; y++)
			{
				// Create some convenient indices
				int i = (y + sy) * pitch + (x + sx);		// This
				int n = (y + sy - 1) * pitch + (x + sx);	// Northern Neighbour
				int s = (y + sy + 1) * pitch + (x + sx);	// Southern Neighbour
				int w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
				int e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

				// If this cell exists, check if it needs edges
				if (world[i].exist)
				{
					// If this cell has no western neighbour, it needs a western edge
					if (!world[w].exist)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (world[n].edge_exist[WEST])
						{
							// Northern neighbour has a western edge, so grow it downwards
							vecEdges[world[n].edge_id[WEST]].end.y += fBlockWidth;
							world[i].edge_id[WEST] = world[n].edge_id[WEST];
							world[i].edge_exist[WEST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							sEdge edge;
							edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
							edge.end.x = edge.start.x; edge.end.y = edge.start.y + fBlockWidth;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[WEST] = edge_id;
							world[i].edge_exist[WEST] = true;
						}
					}

					// If this cell dont have an eastern neignbour, It needs a eastern edge
					if (!world[e].exist)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (world[n].edge_exist[EAST])
						{
							// Northern neighbour has one, so grow it downwards
							vecEdges[world[n].edge_id[EAST]].end.y += fBlockWidth;
							world[i].edge_id[EAST] = world[n].edge_id[EAST];
							world[i].edge_exist[EAST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							sEdge edge;
							edge.start.x = (sx + x + 1) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
							edge.end.x = edge.start.x; edge.end.y = edge.start.y + fBlockWidth;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[EAST] = edge_id;
							world[i].edge_exist[EAST] = true;
						}
					}

					// If this cell doesnt have a northern neignbour, It needs a northern edge
					if (!world[n].exist)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (world[w].edge_exist[NORTH])
						{
							// Western neighbour has one, so grow it eastwards
							vecEdges[world[w].edge_id[NORTH]].end.x += fBlockWidth;
							world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
							world[i].edge_exist[NORTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so create one
							sEdge edge;
							edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
							edge.end.x = edge.start.x + fBlockWidth; edge.end.y = edge.start.y;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[NORTH] = edge_id;
							world[i].edge_exist[NORTH] = true;
						}
					}

					// If this cell doesnt have a southern neignbour, It needs a southern edge
					if (!world[s].exist)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (world[w].edge_exist[SOUTH])
						{
							// Western neighbour has one, so grow it eastwards
							vecEdges[world[w].edge_id[SOUTH]].end.x += fBlockWidth;
							world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
							world[i].edge_exist[SOUTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so I need to create one
							sEdge edge;
							edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y + 1) * fBlockWidth;
							edge.end.x = edge.start.x + fBlockWidth; edge.end.y = edge.start.y;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[SOUTH] = edge_id;
							world[i].edge_exist[SOUTH] = true;
						}
					}

				}

			}
	}

	bool checkLineIntersection(sIntersectResult* point, sEdge& e1, sEdge& e2)
	{
		// Source: http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
		// calculate t1 and t2 where t1 is 

		// calculate denominator 
		float denominator = (e2.end.x - e2.start.x)*(e1.start.y - e1.end.y) - (e1.start.x - e1.end.x)*(e2.end.y - e2.start.y);

		// check for division by zero error
		if (denominator != 0.0f)
		{
			// calculate t1 and t2 values
			float t1 = ((e2.start.y - e2.end.y)*(e1.start.x - e2.start.x) + (e2.end.x - e2.start.x)*(e1.start.y - e2.start.y))
				/ denominator;

			float t2 = ((e1.start.y - e1.end.y)*(e1.start.x - e2.start.x) + (e1.end.x - e1.start.x)*(e1.start.y - e2.start.y))
				/ denominator;

			if ((t1 >= 0.0f && t1 <= 1.0f) && (t2 >= 0.0f && t2 <= 1.0f)) // line segments are intersecting if true
			{
				if (point != nullptr)
				{
					// calculate intersection point
					point->px = e1.start.x + t1 * (e1.end.x - e1.start.x);
					point->py = e1.start.y + t1 * (e1.end.y - e1.start.y);
					point->t = t1;
				}
				return true;
			}
		}

		return false; // no intersection was found
	};


	void MyCalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad = 0.0f) // fovRad = 0.0 is 360 vision
	{
		// Get rid of existing polygon
		vecVisibilityPolygonPoints.clear();

		// For each edge in PolyMap
		for (auto &edge1 : vecEdges)
		{
			// Take the start point, then the end point (we could use a pool of
			// non-duplicated points here, it would be more optimal)
			for (int i = 0; i < 2; i++)
			{
				float raydx, raydy;
				raydx = (i == 0 ? edge1.start.x : edge1.end.x) - ox;
				raydy = (i == 0 ? edge1.start.y : edge1.end.y) - oy;

				float base_ang = atan2f(raydy, raydx);

				float ang = 0;
				// For each point, cast 3 rays, 1 directly at point
				// and 1 a little bit either side
				for (int j = 0; j < 3; j++)
				{
					if (j == 0)	ang = base_ang - 0.0001f;
					if (j == 1)	ang = base_ang;
					if (j == 2)	ang = base_ang + 0.0001f;

					vec2d rayS = { ox, oy };
					vec2d rayE = { radius * cosf(ang) + ox, radius * sinf(ang) + oy };

					sEdge ray = { rayS, rayE };

					float min_t1 = INFINITY;
					float min_px = 0, min_py = 0, min_ang = 0;
					bool bValid = false;

					// Check for ray intersection with all edges
					for (auto &edge2 : vecEdges)
					{
						sIntersectResult result;

						if (checkLineIntersection(&result, ray, edge2))
						{
							if (result.t < min_t1)
							{
								min_t1 = result.t;
								min_px = result.px;
								min_py = result.py;
								min_ang = atan2f(min_py - oy, min_px - ox);
								bValid = true;

							}
						}
					}

					if (bValid) // Add intersection point to visibility polygon perimeter
						vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
					
				}
			}
		}

		// Sort perimeter points by angle from source. This will allow
		// us to draw a triangle fan.
		sort(
			vecVisibilityPolygonPoints.begin(),
			vecVisibilityPolygonPoints.end(),
			[&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
		{
			return get<0>(t1) < get<0>(t2);
		});

	}


	void CalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad = 0.0f) // fovRad = 0.0 is 360 vision
	{
		// Get rid of existing polygon
		vecVisibilityPolygonPoints.clear();

		// For each edge in PolyMap
		for (auto &edge1 : vecEdges)
		{
			// Take the start point, then the end point (we could use a pool of
			// non-duplicated points here, it would be more optimal)
			for (int i = 0; i < 2; i++)
			{
				float raydx, raydy;
				raydx = (i == 0 ? edge1.start.x : edge1.end.x) - ox;
				raydy = (i == 0 ? edge1.start.y : edge1.end.y) - oy;

				float base_ang = atan2f(raydy, raydx);
				
				float ang = 0;
				// For each point, cast 3 rays, 1 directly at point
				// and 1 a little bit either side
				for (int j = 0; j < 3; j++)
				{
					if (j == 0)	ang = base_ang - 0.0001f;
					if (j == 1)	ang = base_ang;
					if (j == 2)	ang = base_ang + 0.0001f;

					float min_t1 = INFINITY;
					float min_px = 0, min_py = 0, min_ang = 0;
					bool bValid = false;

					// Check for ray intersection with all edges
					for (auto &edge2 : vecEdges)
					{
						// Create ray along angle for required distance
						raydx = radius * cosf(ang);
						raydy = radius * sinf(ang);

						// Create line segment vector
						float segmentdx = edge2.end.x - edge2.start.x;
						float segmentdy = edge2.end.y - edge2.start.y;

						// check that ray is not colinear with edge
						if (fabs(segmentdx - raydx) > 0.0f && fabs(segmentdy - raydy) > 0.0f)
						{
							// t2 is normalised distance from line segment start to line segment end of intersect point - distance along the edge
							float t2 = (raydx * (edge2.start.y - oy) + (raydy * (ox - edge2.start.x))) / (segmentdx * raydy - segmentdy * raydx);
							// t1 is normalised distance from source along ray to ray length of intersect point - distance along the ray
							float t1 = (edge2.start.x + segmentdx * t2 - ox) / raydx;

							// If intersect point exists along ray, and along line 
							// segment then intersect point is valid
							if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
							{
								// Check if this intersect point is closest to source. If
								// it is, then store this point and reject others
								if (t1 < min_t1)
								{
									min_t1 = t1;
									min_px = ox + raydx * t1;
									min_py = oy + raydy * t1;
									min_ang = atan2f(min_py - oy, min_px - ox);
									bValid = true;
								}
							}
						}
					}

					if (fovRad > 0.0f)
					{
						if (bValid && (min_ang >= direction - fovRad && min_ang <= direction + fovRad)) // Add intersection point to visibility polygon perimeter
							vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
					}
					else
					{
						if (bValid) // Add intersection point to visibility polygon perimeter
							vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
					}

				}
			}
		}

		// Sort perimeter points by angle from source. This will allow
		// us to draw a triangle fan.
		sort(
			vecVisibilityPolygonPoints.begin(),
			vecVisibilityPolygonPoints.end(),
			[&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
		{
			return get<0>(t1) < get<0>(t2);
		});

		// unless we have 360 degree vision, we need to add a point in the mouse position
		// to the back of the array
		if (fovRad > 0.0f)
		{
			vecVisibilityPolygonPoints.push_back({ 0.0f, GetMouseX(), GetMouseY() });
		}

	}


	bool checkIfEnemyIsVisible(float ox, float oy, float radius, int &retCount)
	{
		float raydx, raydy;
		raydx = radius;
		raydy = 0.0f;

		// count for the number of intersections
		int count = 0;

		// For each edge in PolyMap
		for (int i = 0; i < vecVisibilityPolygonPoints.size(); i++)
		{
			float edgeSx, edgeSy;
			float edgeEx, edgeEy;

			if (i == vecVisibilityPolygonPoints.size() - 1)
			{
				// next point would go over
				edgeSx = get<1>(vecVisibilityPolygonPoints[i]);
				edgeSy = get<2>(vecVisibilityPolygonPoints[i]);

				edgeEx = get<1>(vecVisibilityPolygonPoints[0]);
				edgeEy = get<2>(vecVisibilityPolygonPoints[0]);
				// segment needs to go from last point to the first
			}
			else
			{
				// segment from current point to next point
				edgeSx = get<1>(vecVisibilityPolygonPoints[i]);
				edgeSy = get<2>(vecVisibilityPolygonPoints[i]);

				edgeEx = get<1>(vecVisibilityPolygonPoints[i+1]);
				edgeEy = get<2>(vecVisibilityPolygonPoints[i+1]);

			}
			
			float segmentdx = edgeEx - edgeSx;
			float segmentdy = edgeEy - edgeSy;
			// check that ray is not colinear with edge
			if (fabs(segmentdx - raydx) > 0.0f && fabs(segmentdy - raydy) > 0.0f)
			{
				// t2 is normalised distance from line segment start to line segment end of intersect point - distance along the edge
				float t2 = (raydx * (edgeSy - oy) + (raydy * (ox - edgeSx))) / (segmentdx * raydy - segmentdy * raydx);
				// t1 is normalised distance from source along ray to ray length of intersect point - distance along the ray
				float t1 = (edgeSx + segmentdx * t2 - ox) / raydx;

				// If intersect point exists along ray, and along line 
				// segment then intersect point is valid
				if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
				{
					count++;
				}
			}
		}

		// returns true if odd, false for even

		retCount = count;
		bool ret = count % 2;
		return ret;
	}


public:
	bool OnUserCreate() override
	{
		world = new sCell[nWorldWidth * nWorldHeight];

		// Add a boundary to the world
		for (int x = 1; x < (nWorldWidth - 1); x++)
		{
			world[1 * nWorldWidth + x].exist = true;
			world[(nWorldHeight - 2) * nWorldWidth + x].exist = true;
		}

		for (int x = 1; x < (nWorldHeight - 1); x++)
		{
			world[x * nWorldWidth + 1].exist = true;
			world[x * nWorldWidth + (nWorldWidth - 2)].exist = true;
		}

		sprLightCast = new olc::Sprite("light_cast.png");

		// Create some screen-sized off-screen buffers for lighting effect
		buffLightTex = new olc::Sprite(ScreenWidth(), ScreenHeight());
		buffLightRay = new olc::Sprite(ScreenWidth(), ScreenHeight());
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		float fBlockWidth = 16.0f;
		float fSourceX = GetMouseX();
		float fSourceY = GetMouseY();

		// Set tile map blocks to on or off
		if (GetMouse(0).bReleased)
		{
			// i = y * width + x
			int i = ((int)fSourceY / (int)fBlockWidth) * nWorldWidth + ((int)fSourceX / (int)fBlockWidth);
			world[i].exist = !world[i].exist;
		}

		// Take a region of "TileMap" and convert it to "PolyMap" - This is done
		// every frame here, but could be a pre-processing stage depending on 
		// how your final application interacts with tilemaps
		ConvertTileMapToPolyMap(0, 0, 40, 30, fBlockWidth, nWorldWidth);


		if (GetMouse(1).bHeld)
		{
		
				MyCalculateVisibilityPolygon(fSourceX, fSourceY, 1000.0f, 0.0f);
			
		}

		// Add "enemy" with A-key
		if (GetKey(olc::A).bPressed)
		{
			// add enemy to vector
			vecEnemies.push_back({ fSourceX, fSourceY, false, 0 });
		}

		// Reset vecEnemies with R-key
		if (GetKey(olc::R).bPressed)
		{
			vecEnemies.clear();
		}

		// Drawing
		SetDrawTarget(nullptr);
		Clear(olc::BLACK);

		int nRaysCast = vecVisibilityPolygonPoints.size();

		// Remove duplicate (or simply similar) points from polygon
		auto it = unique(
			vecVisibilityPolygonPoints.begin(),
			vecVisibilityPolygonPoints.end(),
			[&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
		{
			return fabs(get<1>(t1) - get<1>(t2)) < 0.1f && fabs(get<2>(t1) - get<2>(t2)) < 0.1f;
		});

		vecVisibilityPolygonPoints.resize(distance(vecVisibilityPolygonPoints.begin(), it));

		int nRaysCast2 = vecVisibilityPolygonPoints.size();
		DrawString(4, 4, "Rays Cast: " + to_string(nRaysCast) + " Rays Drawn: " + to_string(nRaysCast2));


		// Draw Blocks from TileMap
		for (int x = 0; x < nWorldWidth; x++)
			for (int y = 0; y < nWorldHeight; y++)
			{
				if (world[y * nWorldWidth + x].exist)
					FillRect(x * fBlockWidth, y * fBlockWidth, fBlockWidth, fBlockWidth, olc::BLUE);
			}

		// Draw Edges from PolyMap
		for (auto &e : vecEdges)
		{
			DrawLine(e.start.x, e.start.y, e.end.x, e.end.y, olc::DARK_RED);
			FillCircle(e.start.x, e.start.y, 3, olc::RED);
			FillCircle(e.end.x, e.end.y, 3, olc::RED);
		}


		// If drawing rays, set an offscreen texture as our target buffer
		if (GetMouse(1).bHeld && vecVisibilityPolygonPoints.size() > 1)
		{
			// Clear offscreen buffer for sprite
			SetDrawTarget(buffLightTex);
			Clear(olc::BLACK);

			// Draw "Radial Light" sprite to offscreen buffer, centered around 
			// source location (the mouse coordinates, buffer is 512x512)
			DrawSprite(fSourceX - 255, fSourceY - 255, sprLightCast);

			// Clear offsecreen buffer for rays
			SetDrawTarget(buffLightRay);
			Clear(olc::BLANK);

			// Draw each triangle in fan
			for (int i = 0; i < vecVisibilityPolygonPoints.size() - 1; i++)
			{
				FillTriangle(
					fSourceX,
					fSourceY,

					get<1>(vecVisibilityPolygonPoints[i]),
					get<2>(vecVisibilityPolygonPoints[i]),

					get<1>(vecVisibilityPolygonPoints[i + 1]),
					get<2>(vecVisibilityPolygonPoints[i + 1]));

			}

			// Fan will have one open edge, so draw last point of fan to first
			FillTriangle(
				fSourceX,
				fSourceY,

				get<1>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),
				get<2>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),

				get<1>(vecVisibilityPolygonPoints[0]),
				get<2>(vecVisibilityPolygonPoints[0]));

			// Wherever rays exist in ray sprite, copy over radial light sprite pixels
			SetDrawTarget(nullptr);
			for (int x = 0; x < ScreenWidth(); x++)
				for (int y = 0; y < ScreenHeight(); y++)
					if (buffLightRay->GetPixel(x, y).r > 0)
						Draw(x, y, buffLightTex->GetPixel(x, y));

			// Draw visibility polygon edges
			for (int i = 0; i < vecVisibilityPolygonPoints.size() - 1; i++)
			{
				DrawLine(
					get<1>(vecVisibilityPolygonPoints[i]),
					get<2>(vecVisibilityPolygonPoints[i]),

					get<1>(vecVisibilityPolygonPoints[i + 1]),
					get<2>(vecVisibilityPolygonPoints[i + 1]));
			}

			// polygon will have an open edge, so draw a line from last point to first
			DrawLine(
				get<1>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),
				get<2>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),

				get<1>(vecVisibilityPolygonPoints[0]),
				get<2>(vecVisibilityPolygonPoints[0]),
				olc::MAGENTA
			);
		}

		// Draw Enemies
		
		if (GetMouse(1).bHeld && vecEnemies.size() > 0)
		{
			// check if enemy should be visible
			for (auto &enemy : vecEnemies)
			{
				int num;
				enemy.visible = checkIfEnemyIsVisible(enemy.x, enemy.y, ScreenWidth(), num);
				enemy.count = num;
			}

			// if enemy is visible -> draw it
			for (auto &enemy : vecEnemies)
			{
				if (enemy.visible)
				{
					FillCircle(enemy.x, enemy.y, 6.0f, olc::DARK_CYAN);
					DrawString(enemy.x - 1, enemy.y - 1, to_string(enemy.count));
				}
			}
		}
		
		else if ( vecEnemies.size() > 0 )
		{
			// Draw all enemies regardless
			for (auto &enemy : vecEnemies)
			{
				FillCircle(enemy.x, enemy.y, 6.0f, olc::CYAN);
			}
		}

		// exit with Escape
		if (GetKey(olc::ESCAPE).bReleased)
			return false;
		else
			return true;
	}
};

int main()
{
	ShadowCasting2D demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
}