#include "Level.h"
#include "Physics.h"
#include "GlobalConstants.h"
#include <sstream>
#include <assert.h>
// #include <algorithm>

// Convenient defines for polymap conversion
constexpr unsigned int NORTH = 0;
constexpr unsigned int SOUTH = 1;
constexpr unsigned int EAST = 2;
constexpr unsigned int WEST = 3;

// static initializations
float Level::m_cellSize = 0.0f;
float Level::m_levelHeight = 0.0f;
float Level::m_levelWidth = 0.0f;
float Level::m_levelOffsetX = 0.0f;
float Level::m_levelOffsetY = 0.0f;
Vec2f Level::m_startPosition(0.0f, 0.0f);
int   Level::m_startCoordX = 0;
int   Level::m_startCoordY = 0;
Vec2f Level::m_endPosition(0.0f, 0.0f);
bool  Level::m_instantiated = false;
std::vector<Edge> Level::vec_edges;
int   Level::m_mapCellWidth = 0;
int	  Level::m_mapCellHeight = 0;
Cell* Level::m_map = nullptr;

Level::~Level()
{
}

Level::Level(std::string path, float screenWidth, float screenHeight)
{
	// only one instance of level should exist
	assert(!m_instantiated);
	m_instantiated = true;
	LoadTextures();
	// load level should crash if something goes wrong
	if (!LoadLevel(path, screenWidth, screenHeight))
	{
		std::cout << "something went wrong when loading level\n";
	}

	ConvertTileMapToPolyMap(0, 0, m_mapCellWidth, m_mapCellHeight, m_levelOffsetX, m_levelOffsetY, m_cellSize, m_mapCellWidth);
}

bool Level::LoadLevel(std::string filepath, float screenWidth, float screenHeight)
{
	// open the file
	std::ifstream input(filepath);
	if (!input.is_open())
	{
		std::cout << "Error: " << filepath << " Not found\n";
		input.close();
		return false;
	}
	// read first line as width and height

	std::string line;
	std::getline(input, line);
	std::istringstream iss(line);
	if (!(iss >> m_mapCellWidth >> m_mapCellHeight >> m_cellSize))
	{
		std::cout << "error reading file\n";
		return false;
	};

	m_cellSize *= GAME_SCALE;
	std::cout << line << "\n";
	// create the map array
	m_map = new Cell[m_mapCellWidth * m_mapCellHeight];

	// calculate the size of the level x and y width
	m_levelWidth = m_mapCellWidth * m_cellSize;
	m_levelHeight = m_mapCellHeight * m_cellSize;

	// calculate the x and y offset for centering the lever
	
	m_levelOffsetX = (screenWidth  / 2.0f) - (m_levelWidth  / 2.0f);
	m_levelOffsetY = (screenHeight / 2.0f) - (m_levelHeight / 2.0f);

	// read following lines char by char
	char data;
	for (int y = 0; y < m_mapCellHeight; y++)
	{
		for (int x = 0; x < m_mapCellWidth; x++)

		{
			if (!input.eof())
			{
				// TODO: remove spriteId setting from here
				input >> data;
				switch (data)
				{
				case 'C':
				case 'c':
					m_map[x + y * m_mapCellWidth].isObstacle = true;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::BUILDING;
					break;
				case 'S':
				case 's':
					GetCell(x, y)->isStart = true;
					m_startCoordX = x;
					m_startCoordY = y;
					m_startPosition = Vec2f(x * m_cellSize + (m_cellSize / 2.0f) + m_levelOffsetX,
											y * m_cellSize + (m_cellSize / 2.0f) + m_levelOffsetY);
					m_map[x + y * m_mapCellWidth].isObstacle = false;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::ROAD_RIGHT;
					break;
				case 'E':
				case 'e':
					GetCell(x, y)->isGoal = true;
					m_endPosition = Vec2f(x * m_cellSize + (m_cellSize / 2.0f) + m_levelOffsetX,
										  y * m_cellSize + (m_cellSize / 2.0f) + m_levelOffsetY);
					m_map[x + y * m_mapCellWidth].isObstacle = false;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::ROAD_RIGHT;
					break;
				default:
					m_map[x + y * m_mapCellWidth].isObstacle = false;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::ROAD_RIGHT;
					break;
				}
			}
			// add Cells coordinates
			m_map[x + y * m_mapCellWidth].xCoord = x;
			m_map[x + y * m_mapCellWidth].yCoord = y;
			m_map[x + y * m_mapCellWidth].xPos = x * m_cellSize + m_levelOffsetX;
			m_map[x + y * m_mapCellWidth].yPos = y * m_cellSize + m_levelOffsetY;
		}
	}

	// close the file
	input.close();

	SetLevelTiles();

	return true;
}

// Returns true, if all textures were loaded succesfully
bool Level::LoadTextures()
{
	size_t numSpritesLoaded = 0;

	// lambda function for simplifying the loading
	auto loadTexture = [&](std::string path, SpriteId sprId) {

		m_sprites[(size_t)sprId] = new olc::Sprite(path);
		if (m_sprites[(size_t)sprId] == nullptr)
		{
			std::cout << "Failed to load sprite " << (size_t)sprId << "\n";
		}
		else 
		{
			numSpritesLoaded++;
		}
	};

	// Load the files using the above lambda function
	loadTexture("resources/road_cross.png",		 SpriteId::ROAD_CROSS);
	loadTexture("resources/road_right.png",		 SpriteId::ROAD_RIGHT);
	loadTexture("resources/road_up.png",		 SpriteId::ROAD_UP);
	loadTexture("resources/road_up_right.png",   SpriteId::ROAD_CORNER_UP_RIGHT);
	loadTexture("resources/road_up_left.png",    SpriteId::ROAD_CORNER_UP_LEFT);
	loadTexture("resources/road_down_right.png", SpriteId::ROAD_CORNER_DOWN_RIGHT);
	loadTexture("resources/road_down_left.png",  SpriteId::ROAD_CORNER_DOWN_LEFT);
	loadTexture("resources/road_t_up.png",       SpriteId::ROAD_T_UP);
	loadTexture("resources/road_t_down.png",     SpriteId::ROAD_T_DOWN);
	loadTexture("resources/road_t_right.png",    SpriteId::ROAD_T_RIGHT);
	loadTexture("resources/road_t_left.png",	 SpriteId::ROAD_T_LEFT);
	loadTexture("resources/building.png",		 SpriteId::BUILDING);
	loadTexture("resources/roof.png",			 SpriteId::ROOF);
	loadTexture("resources/goal.png",			 SpriteId::GOAL);


	// Check we loaded all the sprites
	if (numSpritesLoaded != (size_t)SpriteId::COUNT)
	{
		std::cout << "failed to load all sprites, are there some missing?\n"
			<< "loaded: " << numSpritesLoaded << " Count: " << (size_t)SpriteId::COUNT << "\n";
		return false;
	}

	// if textures were loaded fine, return true
	return true;
}

void Level::InitPathfinding()
{

	ClearPathfinding();

	// Create connections - in this case m_map are on a regular grid
	for (int x = 0; x < m_mapCellWidth; x++)
	{
		for (int y = 0; y < m_mapCellHeight; y++)
		{
			// check that cell we are adding is not an obstacle
			// Important for zombie behaviour
			if (!m_map[y * m_mapCellWidth + x].isObstacle)
			{
				if (y > 0)
				{
					if (!m_map[(y - 1) * m_mapCellWidth + (x + 0)].isObstacle)
						m_map[y * m_mapCellWidth + x].vecNeighbours.push_back(&m_map[(y - 1) * m_mapCellWidth + (x + 0)]); // UP
				}
				if (y < m_mapCellHeight - 1)
				{
					if (!m_map[(y + 1) * m_mapCellWidth + (x + 0)].isObstacle)
						m_map[y * m_mapCellWidth + x].vecNeighbours.push_back(&m_map[(y + 1) * m_mapCellWidth + (x + 0)]); // DOWN
				}
				if (x > 0)
				{
					if (!m_map[(y + 0) * m_mapCellWidth + (x - 1)].isObstacle)
						m_map[y * m_mapCellWidth + x].vecNeighbours.push_back(&m_map[(y + 0) * m_mapCellWidth + (x - 1)]); // LEFT
				}
				if (x < m_mapCellWidth - 1)
				{
					if (!m_map[(y + 0) * m_mapCellWidth + (x + 1)].isObstacle)
						m_map[y * m_mapCellWidth + x].vecNeighbours.push_back(&m_map[(y + 0) * m_mapCellWidth + (x + 1)]); // RIGHT
				}

				// We can also connect diagonally
				/*if (y>0 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
				if (y<nMapHeight-1 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
				if (y>0 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
				if (y<nMapHeight - 1 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
				*/
			}
		}
	}
}

void Level::ClearPathfinding()
{
	for (int x = 0; x < m_mapCellWidth; x++)
	{
		for (int y = 0; y < m_mapCellHeight; y++)
		{
			m_map[y * m_mapCellWidth + x].globalGoalDist = INFINITY;
			m_map[y * m_mapCellWidth + x].localGoalDist = INFINITY;
			m_map[y * m_mapCellWidth + x].parentCell = nullptr;
			m_map[y * m_mapCellWidth + x].isVisited = false;
		}
	}
}

void Level::SetLevelTiles()
{

	// this is a convenience map when we start loading road tiles
	std::map<unsigned char, SpriteId> roadTypeSelectionMap;
	roadTypeSelectionMap.emplace(0,  SpriteId::ROAD_CROSS);
	roadTypeSelectionMap.emplace(1,  SpriteId::ROAD_UP);
	roadTypeSelectionMap.emplace(2,  SpriteId::ROAD_UP);
	roadTypeSelectionMap.emplace(3,  SpriteId::ROAD_UP);
	roadTypeSelectionMap.emplace(4,  SpriteId::ROAD_RIGHT);
	roadTypeSelectionMap.emplace(5,  SpriteId::ROAD_CORNER_UP_RIGHT);
	roadTypeSelectionMap.emplace(6,  SpriteId::ROAD_CORNER_DOWN_RIGHT);
	roadTypeSelectionMap.emplace(7,  SpriteId::ROAD_T_RIGHT);
	roadTypeSelectionMap.emplace(8,  SpriteId::ROAD_RIGHT);
	roadTypeSelectionMap.emplace(9,  SpriteId::ROAD_CORNER_UP_LEFT);
	roadTypeSelectionMap.emplace(10, SpriteId::ROAD_CORNER_DOWN_LEFT);
	roadTypeSelectionMap.emplace(11, SpriteId::ROAD_T_LEFT);
	roadTypeSelectionMap.emplace(12, SpriteId::ROAD_RIGHT);
	roadTypeSelectionMap.emplace(13, SpriteId::ROAD_T_UP);
	roadTypeSelectionMap.emplace(14, SpriteId::ROAD_T_DOWN);
	roadTypeSelectionMap.emplace(15, SpriteId::ROAD_CROSS);


	for (int x = 0; x < m_mapCellWidth; x++)
	{
		for (int y = 0; y < m_mapCellHeight; y++)
		{
			Cell* currentCell = GetCell(x, y);
		
			// convenient indices, if index would go out of bounds, set it to nullptr
			Cell* north		  = y > 0				    ? GetCell(x, y - 1) : nullptr;
			Cell* south		  = y < m_mapCellHeight - 1 ? GetCell(x, y + 1) : nullptr;
			Cell* east		  = x < m_mapCellWidth  - 1 ? GetCell(x + 1, y) : nullptr;
			Cell* west		  = x > 0				    ? GetCell(x - 1, y) : nullptr;

			
			// is the tile an obstacle
			if (currentCell->isObstacle)
			{
				// we are placing a building tile here, so we just need to check if the south is an obstacle
				if (south != nullptr && south->isObstacle)
				{
					currentCell->sprId = SpriteId::ROOF;
				}
				else 
				{
					currentCell->sprId = SpriteId::BUILDING;
				}
			}
			else if (currentCell->isGoal)
			{
				// Goal tile
				currentCell->sprId = SpriteId::GOAL;
			}
			else 
			{
				// We are going to do some binary trickery when determing if what kind of 
				// road tile we need
				// basically we check north, south, east and west neighbor for opennes
				// and use an unsigned char to record the result like so
				//	0	 0	   0	 0
				// west east south north

				// then according to the value, we select the corresponding tile
				// from the roadTypeSelectionMap

				unsigned char roadTile = 0; // 00000000b

				if (north != nullptr && !north->isObstacle) 
					roadTile |= (1 << 0);
				if (south != nullptr && !south->isObstacle)
					roadTile |= (1 << 1);
				if (east != nullptr && !east->isObstacle)
					roadTile |= (1 << 2);
				if (west != nullptr && !west->isObstacle)
					roadTile |= (1 << 3);

				// set the current road considering the the value
				currentCell->sprId = roadTypeSelectionMap[roadTile];
			}
		}
	}
}

void Level::ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float offsetX, float offsetY, float fBlockWidth, int pitch)
{
	// Clear "PolyMap"
	vec_edges.clear();
	assert(m_map != nullptr);
	// Clear each cells information
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_map[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
				m_map[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
			}
		}
	}

	// Iterate through region from top left to bottom right
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			// Create some convenient indices
			int i = (y + sy) * pitch + (x + sx);			// This Cell
			int north = (y + sy - 1) * pitch + (x + sx);	// Northern Neighbour
			int south = (y + sy + 1) * pitch + (x + sx);	// Southern Neighbour
			int west = (y + sy) * pitch + (x + sx - 1);		// Western Neighbour
			int east = (y + sy) * pitch + (x + sx + 1);		// Eastern Neighbour
			// Bools for if neighbors are inside the array
			bool i_isBoundary = x == 0 || x + 1 == m_mapCellWidth || y == 0 || y + 1 == m_mapCellHeight;
			bool north_in = y > 0;
			bool south_in = y + 1 < m_mapCellHeight;
			bool west_in  = x > 0;
			bool east_in  = x + 1 < m_mapCellWidth;

			// Whenever we check if a cell has a neighbor, we need to check, if we are checking outside
			// the level bounds. The conditions for, if nswe-neighbors are outside/inside the level (i.e. don't exist) are as follows:
			// West - x == 0 / x > 0
			// East - x + 1 == m_width / x + 1 < m_width
			// North - y == 0 / y > 0
			// South - y + 1 == m_height / y + 1 < m_height

			// If this cell exists, check if it needs edges
			if (m_map[i].isObstacle)
			{
				// If this cell has no western neighbour (or neighbor is outside map), it needs a western edge
				if (!west_in || !m_map[west].isObstacle)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one. Also check we are not accessing data outside the array
					if (north_in && m_map[north].edge_exist[WEST])
					{
						// Northern neighbour has a western edge, so grow it downwards
						vec_edges[m_map[north].edge_id[WEST]].end.y += fBlockWidth;
						m_map[i].edge_id[WEST] = m_map[north].edge_id[WEST];
						m_map[i].edge_exist[WEST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.start.x = (sx + x) * fBlockWidth; 
						edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x; 
						edge.end.y = edge.start.y + fBlockWidth;
						// the normal of the edge needs to point west
						edge.normal.x = -1.0f;
						edge.normal.y = 0.0f;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[WEST] = edge_id;
						m_map[i].edge_exist[WEST] = true;
					}
				}

				// If this cell dont have an eastern neignbour (or neighbor is outside map), It needs a eastern edge
				if (!east_in || !m_map[east].isObstacle)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one. Also check we are not accessing data outside the array
					if (north_in && m_map[north].edge_exist[EAST])
					{
						// Northern neighbour has one, so grow it downwards
						vec_edges[m_map[north].edge_id[EAST]].end.y += fBlockWidth;
						m_map[i].edge_id[EAST] = m_map[north].edge_id[EAST];
						m_map[i].edge_exist[EAST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.start.x = ((sx + x + 1) * fBlockWidth); 
						edge.start.y = ((sy + y) * fBlockWidth);
						edge.end.x = edge.start.x; 
						edge.end.y = edge.start.y + fBlockWidth;
						// Normal of the edge needs to point east
						edge.normal.x = 1.0f;
						edge.normal.y = 0.0f;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[EAST] = edge_id;
						m_map[i].edge_exist[EAST] = true;
					}
				}

				// If this cell doesnt have a northern neignbour (or neighbor is outside map), It needs a northern edge
				if (!north_in || !m_map[north].isObstacle)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one. Also check we are not accessing data outside the array
					if (west_in && m_map[west].edge_exist[NORTH])
					{
						// Western neighbour has one, so grow it eastwards
						vec_edges[m_map[west].edge_id[NORTH]].end.x += fBlockWidth;
						m_map[i].edge_id[NORTH] = m_map[west].edge_id[NORTH];
						m_map[i].edge_exist[NORTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so create one
						Edge edge;
						edge.start.x = (sx + x) * fBlockWidth; 
						edge.start.y = (sy + y) * fBlockWidth ;
						edge.end.x = edge.start.x + fBlockWidth; 
						edge.end.y = edge.start.y;
						// Normal of the edge needs to point north
						edge.normal.x = 0.0f;
						edge.normal.y = -1.0f;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[NORTH] = edge_id;
						m_map[i].edge_exist[NORTH] = true;
					}
				}

				// If this cell doesnt have a southern neignbour (or neighbor is outside map), It needs a southern edge
				if (!south_in || !m_map[south].isObstacle)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one. Also check we are not accessing data outside the array
					if (west_in && m_map[west].edge_exist[SOUTH])
					{
						// Western neighbour has one, so grow it eastwards
						vec_edges[m_map[west].edge_id[SOUTH]].end.x += fBlockWidth;
						m_map[i].edge_id[SOUTH] = m_map[west].edge_id[SOUTH];
						m_map[i].edge_exist[SOUTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so I need to create one
						Edge edge;
						edge.start.x = (sx + x) * fBlockWidth; 
						edge.start.y = (sy + y + 1) * fBlockWidth;
						edge.end.x = edge.start.x + fBlockWidth; 
						edge.end.y = edge.start.y;
						// Normal of the edge needs to point north
						edge.normal.x = 0.0f;
						edge.normal.y = 1.0f;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[SOUTH] = edge_id;
						m_map[i].edge_exist[SOUTH] = true;
					}
				}
			}
		}
	}
#pragma warning (disable : 4244) // narrowing conversion from int32_t to float
	// create the level boundary
	// left

	Edge left;
	left.start.x = sx; 
	left.start.y = sy;
	left.end.x = sx; 
	left.end.y = (sy + m_mapCellHeight) * fBlockWidth;
	left.normal.x = 1.0f; 
	left.normal.y = 0.0f;
	vec_edges.push_back(left);

	// right
	Edge right;
	right.start.x = (sx + m_mapCellWidth) * fBlockWidth; 
	right.start.y = sy + offsetY;
	right.end.x = (sx + m_mapCellWidth) * fBlockWidth; 
	right.end.y = (sy + m_mapCellHeight) * fBlockWidth;
	right.normal.x = -1.0f; 
	right.normal.y = 0.0f;
	vec_edges.push_back(right);

	// top
	Edge top;
	top.start.x = sx; 
	top.start.y = sy;
	top.end.x = (sx + m_mapCellWidth) * fBlockWidth; 
	top.end.y = sy;
	top.normal.x = 0.0f; 
	top.normal.y = 1.0f;
	vec_edges.push_back(top);
	
	// bottom
	Edge bottom;
	bottom.start.x = sx; 
	bottom.start.y = (sy + m_mapCellHeight) * fBlockWidth;
	bottom.end.x = (sx + m_mapCellWidth) * fBlockWidth; 
	bottom.end.y = (sy + m_mapCellHeight) * fBlockWidth;
	bottom.normal.x = 0.0f; 
	bottom.normal.y = -1.0f;
	vec_edges.push_back(bottom);

	// Finally go through the edges one more time and add the calculated
	// level offsets to the coordinates
	for (auto& edge : vec_edges)
	{
		edge.start.x += offsetX;
		edge.start.y += offsetY;
		edge.end.x += offsetX;
		edge.end.y += offsetY;
	}

#pragma warning (default : 4244)

}


void Level::DrawPolyMap(olc::PixelGameEngine & engine)
{
#pragma warning (disable : 4244) // Narrowing conversion from float to int32_t
	for (auto e : vec_edges)
	{
		engine.DrawLine(e.start.x, e.start.y, e.end.x, e.end.y, olc::DARK_RED);
		engine.DrawCircle(e.start.x, e.start.y, 5, olc::GREEN);
		engine.FillCircle(e.end.x, e.end.y, 3, olc::RED);
		// Draw the normal
		Vec2f edgeCenter = { e.start.x + (e.end.x - e.start.x)*0.5f, e.start.y + (e.end.y-e.start.y)*0.5f };

		engine.FillCircle(edgeCenter.x, edgeCenter.y, 3, olc::BLUE);
		engine.DrawLine(edgeCenter.x, edgeCenter.y, edgeCenter.x + e.normal.x*10.0f, edgeCenter.y + e.normal.y*10.0f, olc::BLUE);
	}

#pragma warning (default : 4244)
}

void Level::DrawConnections(olc::PixelGameEngine& engine)
{
	for (int y = 0; y < m_mapCellHeight; y++)
	{
		for (int x = 0; x < m_mapCellWidth; x++)
		{
			for (auto itr : m_map[y * m_mapCellWidth + x].vecNeighbours)
			{
				Vec2f cellCenter = GetCellCenterPos(x, y);
				Vec2f neighborCenter = GetCellCenterPos(itr->xCoord, itr->yCoord);

#pragma warning (disable : 4244) // Conversion from T to int32_t
				engine.DrawLine( cellCenter.x, cellCenter.y,
								 neighborCenter.x, neighborCenter.y,
								 olc::WHITE, (uint32_t)0xF0F0F0F0F);
#pragma warning (default : 4244) 
			}
		}
	}
}

/** NOT IMPLEMENTED

NOTE: Currently this isn't implemented in the project. Might implementit in a later time
void Level::CalculateVisibilityPolygon(float ox, float oy, float radius, float direction, float fovRad)
{
	// Get rid of existing polygon
	m_vecVisibilityPolygonPoints.clear();

	// float dirPolar = Vec2f::PolarAngle({ cosf(direction), sinf(direction) });
	// reference vector will be used for sorting, we just need the direction
	Vec2f rayRef = { cosf(direction - fovRad), sinf(direction - fovRad) };


	// For each edge in PolyMap
	for (auto& edge1 : vec_edges)
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

				Vec2f rayS = { ox, oy };
				Vec2f rayE = { radius * cosf(ang) + ox, radius * sinf(ang) + oy };

				Ray ray = { rayS, rayE };

				float min_t1 = INFINITY;
				float min_px = 0, min_py = 0, min_ang = 0;
				bool bValid = false;

				// Check for ray intersection with all edges
				for (auto& edge2 : vec_edges)
				{
					IntersectResult result;

					if (Physics::CheckLineIntersection(ray, edge2, &result))
					{
						if (result.t < min_t1)
						{
							min_t1 = result.t;
							min_px = result.px;
							min_py = result.py;
							// angle relative to left ray
							min_ang = Vec2f::AngleBetween(rayRef, { min_px - ox, min_py - oy });

							// min_ang = Vec2f::PolarAngle({ min_px - ox, min_py - oy });
							//min_ang = atan2f(min_py - oy, min_px - ox);
							bValid = true;

						}
					}
				}

				if (bValid)
				{
					// source: https://gamedev.stackexchange.com/questions/100504/how-do-i-optimize-2d-visibility-cone-calculations
					// Use cross product to determine if, point is within the area 
					if (Vec2f::IsLeft({ ox,oy }, { cosf(direction - fovRad) + ox, sinf(direction - fovRad) + oy }, { min_px, min_py })
						&& !Vec2f::IsLeft({ ox,oy }, { cosf(direction + fovRad) + ox, sinf(direction + fovRad) + oy }, { min_px, min_py }))
						m_vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
				}

			}
		}
	}

	// SHOOT THE EXTRA RAYS
	for (int i = 0; i < 2; i++)
	{
		// if i == 0, shoot left, if i == 1, shoot right
		float dirRad = i % 2 == 0 ? direction - fovRad : direction + fovRad;

		Vec2f rayS = { ox, oy };
		Vec2f rayE = { radius * cosf(dirRad) + ox, radius * sinf(dirRad) + oy };

		Ray ray = { rayS, rayE };

		float min_t1 = INFINITY;
		float min_px = 0, min_py = 0, min_ang = 0;
		bool bValid = false;

		// Check for ray intersection with all edges
		for (auto& edge : vec_edges)
		{
			IntersectResult result;

			if (Physics::CheckLineIntersection( ray, edge, &result))
			{
				if (result.t < min_t1)
				{
					min_t1 = result.t;
					min_px = result.px;
					min_py = result.py;
					min_ang = Vec2f::AngleBetween(rayRef, { min_px - ox, min_py - oy });

					//min_ang = Vec2f::PolarAngle({ min_px - ox, min_py - oy });
					//min_ang = atan2f(min_py - oy, min_px - ox);
					bValid = true;
				}
			}
		}

		if (bValid)
		{
			m_vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
		}
	}

	// Sort perimeter points by angle from source. This will allow
	// us to draw a triangle fan.
	// when dealing with conical vision, we must sort relative to the left ray


	std::sort(
		m_vecVisibilityPolygonPoints.begin(),
		m_vecVisibilityPolygonPoints.end(),
		[&](const std::tuple<float, float, float>& t1, const std::tuple<float, float, float>& t2)
		{
			return std::get<0>(t1) < std::get<0>(t2);
		});


	// unless we have 360 degree vision, we need to add a point in the mouse position
	// to the back of the array
	m_vecVisibilityPolygonPoints.push_back({ Vec2f::PolarAngle({cosf(direction) + ox, sinf(direction) + oy }), ox, oy });
}
*/

void Level::DrawLevel(olc::PixelGameEngine & engine)
{
#pragma warning (disable : 4244) // Narrowing conversion from float to int32_t

	// Draw Blocks from TileMap
	for (int x = 0; x < m_mapCellWidth; x++)
	{
		for (int y = 0; y < m_mapCellHeight; y++)
		{
			Cell* currentCell = GetCell(x, y);
			engine.DrawSprite(currentCell->xPos, currentCell->yPos,
				m_sprites[(size_t)currentCell->sprId]);

			if (currentCell->isStart)
			{
				engine.DrawString(currentCell->xPos, currentCell->yPos, "Start", olc::BLUE, GAME_SCALE);
			}
		}
	}
#pragma warning (default : 4244)

}

std::vector<std::pair<int, int>> Level::GetPathToTarget(const Vec2f& start, const Vec2f& target)
{
	// get relevant cell coordinates of start and target
	Cell* startCell  = GetCell(start);
	Cell* targetCell = GetCell(target);
	// Turn them into pairs of cell coordinates
	std::pair<int, int> startCoord  = std::make_pair(startCell->xCoord , startCell->yCoord);
	std::pair<int, int> targetCoord = std::make_pair(targetCell->xCoord, targetCell->yCoord);

	// call solve AStar

	std::vector<std::pair<int, int>> ret = SolveAStarPath(startCoord, targetCoord);

	return ret;
	
}

std::vector<std::pair<int, int>> Level::SolveAStarPath(std::pair<int, int> startCoord, std::pair<int, int> targetCoord)
{
	// Reset Navigation Graph - default all node states
	ClearPathfinding();

#pragma warning (disable : 4244)
	auto distance = [](Cell* a, Cell* b) // For convenience
	{
		return sqrtf((a->xCoord - b->xCoord) * (a->xCoord - b->xCoord) + (a->yCoord - b->yCoord) * (a->yCoord - b->yCoord));
	};

	auto heuristic = [distance](Cell* a, Cell* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};
#pragma warning (default : 4244)

	// Setup starting conditions
	
	Cell* nodeStart = GetCell(startCoord.first, startCoord.second);
	Cell* nodeEnd = GetCell(targetCoord.first, targetCoord.second);

	Cell* nodeCurrent = nodeStart;
	nodeStart->localGoalDist = 0.0f;
	nodeStart->globalGoalDist = heuristic(nodeStart, nodeEnd);

	// Add start node to not tested list - this will ensure it gets tested.
	// As the algorithm progresses, newly discovered nodes get added to this
	// list, and will themselves be tested later
	std::list<Cell*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	// if the not tested list contains nodes, there may be better paths
	// which have not yet been explored. However, we will also stop 
	// searching when we reach the target - there may well be better
	// paths but this one will do - it wont be the longest.
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const Cell* lhs, const Cell* rhs) { return lhs->globalGoalDist < rhs->globalGoalDist; });

		// Front of listNotTestedNodes is potentially the lowest distance node. Our
		// list may also contain nodes that have been visited, so ditch these...
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->isVisited)
			listNotTestedNodes.pop_front();

		// ...or abort because there are no valid nodes left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->isVisited = true; // We only explore a node once


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->isVisited && nodeNeighbour->isObstacle == false)
				listNotTestedNodes.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest parent distance
			float fPossiblyLowerGoal = nodeCurrent->localGoalDist + distance(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->localGoalDist)
			{
				nodeNeighbour->parentCell = nodeCurrent;
				nodeNeighbour->localGoalDist = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->globalGoalDist = nodeNeighbour->localGoalDist + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}
	// Create the returnable list
	std::vector<std::pair<int, int>> returnableList;
	// populate the path by starting at the end, and following the parent node trail
	// back to the start - the start node will not have a parent path to follow
	if (nodeEnd != nullptr)
	{
		Cell* p = nodeEnd;
		while (p->parentCell != nullptr)
		{
			returnableList.push_back(std::make_pair(p->xCoord, p->yCoord));
			// Set next node to this node's parent
			p = p->parentCell;
		}
	}

	return returnableList;
}

Cell* Level::GetCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_mapCellWidth) x = m_mapCellWidth - 1;
	if (y < 0) y = 0;
	if (y >= m_mapCellHeight) y = m_mapCellHeight - 1;

	return &m_map[y * m_mapCellWidth + x];
}

Cell* Level::GetCell(Vec2f pos)
{
	int CellX = (int)((pos.x - m_levelOffsetX) / m_cellSize);
	int CellY = (int)((pos.y - m_levelOffsetY) / m_cellSize);

	return GetCell(CellX, CellY);
}
