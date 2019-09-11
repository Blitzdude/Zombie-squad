#pragma once
#include "Vec2.h"
#include "StructCollection.h"
#include "olcPixelGameEngine.h"
#include <string>
#include <map>
#include <array>

/*
Level
	- holds all data for the map
	- Tile data
	- Edge data
	- Handles pathfindng from one point to another (A* star)
	- map of sprites needed to draw <id, olc::sprite>
*/

/// Sprite ID

/*  This is used to determine by level, which sprite to draw in the cell
*/

enum class SpriteId
{
	ROAD = 0,
	BUILDING,
	COUNT
};

/// Cell structure

/* Game world is composed of Cells. They hold edge information and other information
*/
struct Cell
{	
	int edge_id[4] = {-1, -1, -1, -1};
	bool edge_exist[4] = {false, false, false, false};
	bool obstacle = false;
	bool isStart = false;
	bool isGoal = false;
	int xPos;
	int yPos;
	SpriteId sprId;
};


/**
* Level short description
*
* The level uses stuff and quarks
*/
class Level
{
public:
	// size of cell
	// map width, height in cells
	
	// cell array
	// edge array
	// visibility polygon calculation
	// A* pathfinding routines

	Level(std::string path);
	~Level();

	bool LoadLevel(std::string filepath);
	bool LoadTextures();
	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch);

	void DrawPolyMap(olc::PixelGameEngine& engine);
	void CalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad); // Not Implemented
	void DrawVisibilityPolygon(const std::vector<std::tuple<float, float, float>>& points);
	// bool CheckLineIntersection(IntersectResult* point, Ray& e1, Edge& e2);
	bool CheckIfVisible(float ox, float oy, float radius); // Not implemented
	void DrawLevel(olc::PixelGameEngine& engine);
	
	// Getters
	float GetCellSize() { return m_cellSize; }
	const std::vector<Edge>& GetEdges() const { return vec_edges; }
	const Vec2f& GetStart() { return m_startPosition; }
	int GetStartX() { return (int)(m_startPosition.x / m_cellSize); }
	int GetStartY() { return (int)(m_startPosition.y / m_cellSize); }

	int GetNumCellsX() { return m_mapCellWidth; }
	int GetNumCellsY() { return m_mapCellHeight; }
	Cell* GetCell(int x, int y);
	Cell* GetCell(Vec2f pos);
	
private:
	int m_mapCellWidth; // Map width in cells
	int m_mapCellHeight; // Map Height in cells
	float m_cellSize;
	Vec2f m_startPosition;
	Cell* m_map; // TODO: This is not very c++11
	std::array<olc::Sprite*, (size_t)SpriteId::COUNT> m_sprites;
	std::vector<std::tuple<float, float, float>> m_vecVisibilityPolygonPoints;
	std::vector<Edge> vec_edges;
};

