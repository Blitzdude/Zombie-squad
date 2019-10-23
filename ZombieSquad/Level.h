#pragma once
#include "Vec2.h"
#include "StructCollection.h"
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

/** 
 * This is used by level to determine, which sprite to draw in the cell
 */

/** 
 * SpriteId Enum class
 *
 * SpriteId is a numbered Enum class by level to determine the Sprite to draw on the cell
 */ 
enum class SpriteId
{
	NONE = -1,
	ROAD_CROSS,
	ROAD_RIGHT,
	ROAD_UP,
	ROAD_CORNER_UP_RIGHT,
	ROAD_CORNER_UP_LEFT,
	ROAD_CORNER_DOWN_RIGHT,
	ROAD_CORNER_DOWN_LEFT,
	ROAD_T_UP,
	ROAD_T_DOWN,
	ROAD_T_RIGHT,
	ROAD_T_LEFT,
	BUILDING,
	ROOF,
	GOAL,
	COUNT
};


/** 
* Cell struct for cell and pathfinding information
*
* Game world is composed of Cells. Each cell holds all necessary
* information used by the level and pathfinding 
*/
struct Cell
{	
public:
	int edge_id[4] = {-1, -1, -1, -1};
	bool edge_exist[4] = {false, false, false, false};
	bool isObstacle = false;
	bool isStart = false;
	bool isGoal = false;
	bool isVisited = false;

	int xCoord = -1;
	int yCoord = -1;
	// upper-left corner
	float xPos = 0.0f;
	float yPos = 0.0f;
	float globalGoalDist = 0.0f;
	float localGoalDist = 0.0f;
	SpriteId sprId = SpriteId::NONE;
	Cell* parentCell = nullptr;
	std::vector<Cell*> vecNeighbours;

};

class ZombieSquad;

/**
* Level class
*
* Level is responsible for everything involved with the level: 
* SpriteId's, Pathfinding etc. 
*
* Level is a static class, meaning it has static methods, that can be
* accessed by any object in the game. These methods are related to getting 
* current cell, level size and etc. 
*
* Only on instance of Level should ever exist, and game should break on an
* assert if an instance of class already exists, when the constructor is called
* Level doesn't use a singleton pattern, because it doesn't have a private constructor
* and it's methods are accessed through static calls with Level:: namespace
*/
class Level
{
public:

	/**
	 * Level construtor
	 * 
	 * constructs the level object and initializes level data
	 * @param path path to level level.txt file
	 * @param screenWidth, screenHeight width and height of the screen used for centering the level
	 */
	Level(std::string path, float screenWidth, float screenHeight);
	~Level() {};

	bool LoadLevel(std::string filepath, float screenWidth, float screenHeight);
	bool LoadTextures();

	/// Initialize Path finding 
	/*
	* NOTE: Must be called AFTER LoadLevel
	* Clears cells for pathfinding and creates connections
	*/
	void InitPathfinding();

	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float offsetX, float offsetY, float fBlockWidth, int pitch);

	void DrawPolyMap(ZombieSquad& game);
	void DrawConnections(ZombieSquad& game);

	/** NOT IMPLEMENTED
	void CalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad); 
	*/

	/** NOT IMPLEMENTED
	void DrawVisibilityPolygon(const std::vector<std::tuple<float, float, float>>& points); 
	*/
	
	/** NOT IMPLEMENTED
	bool CheckIfVisible(float ox, float oy, float radius); // Not implemented
	*/
	
	/**
	 * Draws the level
	 *
	 * Used for drawing the level
	 * @param engine Reference to the game engine class
	 */
	void DrawLevel(ZombieSquad& game);
	
	/**
	 * Gets path as vector of int pairs
	 *
	 * Uses A*-algorithm to find the path from start location to target location
	 * @param start Position of the start coordinate
	 * @returns returns a vector of int pairs corresponding to cells to pass through
	 * @param target Position of the target location
	 */
	static std::vector<std::pair<int, int>> GetPathToTarget(const Vec2f& start, const Vec2f& target);

	/// Gets the size of a single cell
	static const float GetCellSize() { return m_cellSize; }
	/// Gets the level coordinate of the level start
	static const Vec2f& GetStartPosition() { return m_startPosition; }
	/// Gets the level coordinate of the level goal
	static const Vec2f& GetEndPosition() { return m_endPosition; }
	/// gets the center position of a single cell
	static const Vec2f GetCellCenterPos(int x, int y) 
	{ 
		return Vec2f((x * m_cellSize + m_cellSize / 2.0f) + m_levelOffsetX,
					 (y * m_cellSize + m_cellSize / 2.0f) + m_levelOffsetY); 
	};
	/// Gets all edges of the level 
	static const std::vector<Edge>& GetEdges() { return vec_edges; }
	/// Gets the cell x-coordinate of start
	static int GetStartCoordX() { return m_startCoordX; }
	/// Gets the cell y-coordinate of start
	static int GetStartCoordY() { return m_startCoordY; }
	static int GetNumCellsX() { return m_mapCellWidth; }
	static int GetNumCellsY() { return m_mapCellHeight; }
	static Cell* GetCell(int x, int y);
	static Cell* GetCell(Vec2f pos);

private:

	/// Clears pathfinding data on cells
	/// 
	/// Needed to be called, whenever pathfinding is done
	static void ClearPathfinding();
	
	/*
	* Sets the levels tile id's
	*
	*
	*
	*/
	void SetLevelTiles();

	/// Solves a path using A-star
	/// 
	/// called by GetPathToTarget(const Vec2f&, const Vec2f&) to solve the path from starting
	/// cell and target cell. 
	/// @param startCoord coordinate of the starting cell
	/// @param targetCoord coordinate of the target cell
	/// @returns returns vector of int pairs corresponding to cells to pass through
	static std::vector<std::pair<int, int>> SolveAStarPath(std::pair<int, int> startCoord, std::pair<int, int> targetCoord);
private:
	static bool m_instantiated;

	static int m_mapCellWidth; // Map width in cells
	static int m_mapCellHeight; // Map Height in cells
	static float m_cellSize;
	static float m_levelWidth;
	static float m_levelHeight;
	static float m_levelOffsetX;
	static float m_levelOffsetY;
	static Vec2f m_startPosition;
	static int m_startCoordX;
	static int m_startCoordY;
	static Vec2f m_endPosition;
	static std::vector<Edge> vec_edges;
	static Cell* m_map;
	std::array<olc::Sprite*, (size_t)SpriteId::COUNT> m_sprites;
	std::vector<std::tuple<float, float, float>> m_vecVisibilityPolygonPoints;
};

