#include "Level.h"
#include <sstream>

// Convenient defines for polymap conversion
constexpr unsigned int NORTH = 0;
constexpr unsigned int SOUTH = 1;
constexpr unsigned int EAST = 2;
constexpr unsigned int WEST = 3;

Level::~Level()
{
}

Level::Level(std::string path)
	: m_startPosition(0.0f, 0.0f)
{
	LoadTextures();
	LoadLevel(path);
	ConvertTileMapToPolyMap(0, 0, m_mapCellWidth, m_mapCellHeight, m_cellSize, m_mapCellWidth);
}

bool Level::LoadLevel(std::string filepath)
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
		std::cout << "error reading file";
		return false;
	};
	std::cout << line;
	// create the map array
	m_map = new Cell[m_mapCellWidth * m_mapCellHeight];

	// read following lines char by char
	char data;
	for (int y = 0; y < m_mapCellHeight; y++)
	{
		for (int x = 0; x < m_mapCellWidth; x++)

		{
			if (!input.eof())
			{
				input >> data;
				switch (data)
				{
				case 'C':
				case 'c':
					m_map[x + y * m_mapCellWidth].obstacle = true;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::BUILDING;
					break;
				case 'S':
				case 's':
					GetCell(x, y)->isStart = true;
					m_startPosition = Vec2f(x * m_cellSize + (m_cellSize / 2), y * m_cellSize + (m_cellSize / 2));
					m_map[x + y * m_mapCellWidth].obstacle = false;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::ROAD;
					break;
				case 'E':
				case 'e':
					GetCell(x, y)->isGoal = true;
					m_map[x + y * m_mapCellWidth].obstacle = false;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::ROAD;
					break;
				default:
					m_map[x + y * m_mapCellWidth].obstacle = false;
					m_map[x + y * m_mapCellWidth].sprId = SpriteId::ROAD;
					break;
				}
			}
		}
	}

	// close the file
	input.close();
	return true;
}

// Returns true, if all textures were loaded succesfully
bool Level::LoadTextures()
{
	m_sprites[(size_t)SpriteId::ROAD] = new olc::Sprite("resources/road.png");
	if (m_sprites[(size_t)SpriteId::ROAD] == nullptr)
	{
		return false;
	}

	m_sprites[(size_t)SpriteId::BUILDING] = new olc::Sprite("resources/building.png");
	if (m_sprites[(size_t)SpriteId::BUILDING] == nullptr)
	{
		return false;
	}

	// if textures were loaded fine, return true
	return true;
}



void Level::ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch)
{
	// Clear "PolyMap"
	vec_edges.clear();

	// Clear each cells information
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			for (int j = 0; j < 4; j++)
			{
				m_map[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
				m_map[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
			}

	// Iterate through region from top left to bottom right
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			// Create some convenient indices
			int i = (y + sy) * pitch + (x + sx);		// This
			int north = (y + sy - 1) * pitch + (x + sx);	// Northern Neighbour
			int south = (y + sy + 1) * pitch + (x + sx);	// Southern Neighbour
			int west = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
			int east = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour
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
			if (m_map[i].obstacle)
			{
				// If this cell has no western neighbour (or neighbor is outside map), it needs a western edge
				if (!west_in || !m_map[west].obstacle)
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
						edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x; edge.end.y = edge.start.y + fBlockWidth;
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
				if (!east_in || !m_map[east].obstacle)
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
						edge.start.x = (sx + x + 1) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x; edge.end.y = edge.start.y + fBlockWidth;
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
				if (!north_in || !m_map[north].obstacle)
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
						edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x + fBlockWidth; edge.end.y = edge.start.y;
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
				if (!south_in || !m_map[south].obstacle)
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
						edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y + 1) * fBlockWidth;
						edge.end.x = edge.start.x + fBlockWidth; edge.end.y = edge.start.y;
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
	// create the level boundary
	// left
	Edge left;
	left.start.x = sx; left.start.y = sy;
	left.end.x = sx; left.end.y = (sy + m_mapCellHeight) * fBlockWidth;
	left.normal.x = 1.0f; left.normal.y = 0.0f;
	vec_edges.push_back(left);

	// right
	Edge right;
	right.start.x = (sx + m_mapCellWidth) * fBlockWidth; right.start.y = sy;
	right.end.x = (sx + m_mapCellWidth) * fBlockWidth; right.end.y = (sy + m_mapCellHeight) * fBlockWidth;
	right.normal.x = -1.0f; right.normal.y = 0.0f;
	vec_edges.push_back(right);

	// top
	Edge top;
	top.start.x = sx; top.start.y = sy;
	top.end.x = (sx + m_mapCellWidth) * fBlockWidth; top.end.y = sy;
	top.normal.x = 0.0f; top.normal.y = 1.0f;
	vec_edges.push_back(top);
	
	// bottom
	Edge bottom;
	bottom.start.x = sx; bottom.start.y = (sy + m_mapCellHeight) * fBlockWidth;
	bottom.end.x = (sx + m_mapCellWidth) * fBlockWidth; bottom.end.y = (sy + m_mapCellHeight) * fBlockWidth;
	bottom.normal.x = 0.0f; bottom.normal.y = -1.0f;
	vec_edges.push_back(bottom);
}


void Level::DrawPolyMap(olc::PixelGameEngine & engine)
{
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
}

void Level::DrawLevel(olc::PixelGameEngine & engine)
{
	// Draw Blocks from TileMap
	for (int x = 0; x < m_mapCellWidth; x++)
	{
		for (int y = 0; y < m_mapCellHeight; y++)
		{
			engine.DrawSprite(x * m_cellSize, y * m_cellSize,
				m_sprites[(size_t)m_map[y * m_mapCellWidth + x].sprId]);

			if (GetCell(x, y)->isStart)
			{
				engine.DrawString(x * m_cellSize, y * m_cellSize, "Start", olc::BLUE);
			}
			if (GetCell(x, y)->isGoal)
			{
				engine.DrawString(x * m_cellSize, y * m_cellSize, "End", olc::BLUE);
			}
		}
	}
}

bool Level::CheckVictory(Actor* m_player)
{
	if (GetCell(m_player->GetPosition())->isGoal)
	{
		return true;
	}
	else 
	{
		return false;
	}
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
	int CellX = (int)(pos.x / m_cellSize);
	int CellY = (int)(pos.y / m_cellSize);

	return GetCell(CellX, CellY);
}
