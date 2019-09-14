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


/// Sprite id Enum class
/// 
/// Sprite id is a numbered Enum class by level to determine the Sprite to draw on the cell
enum class SpriteId
{
	NONE = -1,
	ROAD,
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
	bool isVisited = false;

	int xCoord = -1;
	int yCoord = -1;
	float globalGoalDist = 0.0f;
	float localGoalDist = 0.0f;
	SpriteId sprId = SpriteId::NONE;
	Cell* parentCell = nullptr;
	std::vector<Cell*> vecNeighbours;
};



/**
* Level class
*
* Level is responsible for everything involved with the level 
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

	/// Initialize Path finding 
	/*
	* NOTE: Must be called AFTER LoadLevel
	* Clears cells for pathfinding and creates connections
	*/
	void InitPathfinding();

	/// Clears pathfinding data on cells
	/// 
	/// Needed to be called, whenever pathfinding is done
	void ClearPathfinding();

	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch);

	void DrawPolyMap(olc::PixelGameEngine& engine);
	void DrawConnections(olc::PixelGameEngine& engine);
	/// Not Implemented
	///
	///
	void CalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad); 
	
	/// Not Implemented
	///
	///
	void DrawVisibilityPolygon(const std::vector<std::tuple<float, float, float>>& points); 
	// bool CheckLineIntersection(IntersectResult* point, Ray& e1, Edge& e2);
	
	/// Not Implemented
	///
	///
	bool CheckIfVisible(float ox, float oy, float radius); // Not implemented
	/// Draws the level
	///
	/// Used for drawing the level
	/// @param engine Reference to the game engine class
	void DrawLevel(olc::PixelGameEngine& engine);
	
	/// Gets path as vector of int pairs
	///
	/// Uses A*-algorithm to find the path from start location to target location
	/// @param start Position of the start coordinate
	/// @param target Position of the target location
	/// @returns returns a vector of int pairs corresponding to cells to pass through
	std::vector<std::pair<int, int>> GetPathToTarget(const Vec2f& start, const Vec2f& target);

	/// Solves a path using A-star
	/// 
	/// called by GetPathToTarget(const Vec2f&, const Vec2f&) to solve the path from starting
	/// cell and target cell. 
	/// @param startCoord coordinate of the starting cell
	/// @param targetCoord coordinate of the target cell
	/// @returns returns vector of int pairs corresponding to cells to pass through
	std::vector<std::pair<int, int>> SolveAStarPath(std::pair<int, int> startCoord, std::pair<int, int> targetCoord);

	// Getters
	float GetCellSize() { return m_cellSize; }
	const std::vector<Edge>& GetEdges() const { return vec_edges; }
	const Vec2f& GetStart() { return m_startPosition; }
	const Vec2f& GetEnd() { return m_endPosition; }
	const Vec2f GetCellCenterPos(int x, int y) 
	{ 
		return Vec2f(x * GetCellSize() + GetCellSize() / 2.0f, y * GetCellSize() + GetCellSize() / 2.0f); 
	};
	
	// TODO: Change naming scheme to following: 
	// - Cell -> Returns Cell*
	// - Pos -> Returns Vec2f as Cells center?
	// - CoordX/CoordY -> returns Cells coordinate int in array
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
	Vec2f m_endPosition;
	Cell* m_map; // TODO: This is not very c++11
	std::array<olc::Sprite*, (size_t)SpriteId::COUNT> m_sprites;
	std::vector<std::tuple<float, float, float>> m_vecVisibilityPolygonPoints;
	std::vector<Edge> vec_edges;
};

