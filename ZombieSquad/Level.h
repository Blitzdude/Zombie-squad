#pragma once
#include "Vec2.h"
#include "olcPixelGameEngine.h"
#include <string>

/*
Level
	- holds all data for the map
	- Tile data
	- Edge data
	- Handles pathfindng from one point to another (A* star)
	- map of sprites needed to draw <id, olc::sprite>
*/

struct Cell
{
	Cell() // default constructor
		: x(0), y(0), walkable(false) {}
	Cell(int p_x, int p_y, bool p_walkable)
		: x(p_x), y(p_y), walkable(p_walkable) {}
	// position in array
	int x;
	int y;
	bool walkable;
	// id of the sprite to draw
};

struct Edge
{
	Vec2f start;
	Vec2f end;
	Vec2f normal;
};

class Level
{
public:
	// size of cell
	// map width, height in cells
	
	// cell array
	// edge array
	// visibility polygon calculation
	// A* pathfinding routines
	bool LoadLevel(std::string filepath);
	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch);
	void CalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad);
	bool checkIfVisible(float ox, float oy, float radius);


	Level();
	~Level();
private:
	int m_mapWidth;
	int m_mapHeight;
};

