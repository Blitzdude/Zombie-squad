#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <string>

struct sVec2
{
	float x, y;
};

struct sEdge
{
	sVec2 start;
	sVec2 end;
	int id;
};

bool checkEdgeIntersection(sVec2* point, const sEdge& e1, const sEdge& e2)
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
				point->x = e1.start.x + t1 * (e1.end.x - e1.start.x);
				point->y = e1.start.y + t1 * (e1.end.y - e1.start.y);
			}
			return true;
		}
	}
	
	return false; // no intersection was found
}

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		srand(time(NULL));

		// create a number of edges
		const unsigned int NUM_EDGES = 50;
		int id = 0;
		for (int i = 0; i < NUM_EDGES; i++)
		{
			sVec2 s = {rand() % ScreenWidth(), rand() % ScreenHeight()};
			sVec2 e = {rand() % ScreenWidth(), rand() % ScreenHeight() };
			vecEdges.push_back({ s, e , id++});
		}

		for (auto &edge1 : vecEdges)
		{
			// check each edge against other edge
			for (auto edge2 : vecEdges)
			{
				// filter out self checking
				if (edge1.id == edge2.id)
				{
					continue;
				}
				
				sVec2 point;

				if (checkEdgeIntersection(&point, edge1, edge2))
				{
					vecPoints.push_back(point);
				}
			}
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// draw Edges
		for (auto& edge : vecEdges)
		{
			DrawLine(edge.start.x, edge.start.y, edge.end.x, edge.end.y, olc::DARK_CYAN);
		}
		// draw Intersection points
		for (auto& point : vecPoints)
		{
			DrawCircle(point.x, point.y, 2, olc::MAGENTA);
		}

		return true;
	}

	// Member variables
	std::vector<sEdge> vecEdges;
	std::vector<sVec2> vecPoints;

};


int main()
{
	Example demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();

	return 0;
}