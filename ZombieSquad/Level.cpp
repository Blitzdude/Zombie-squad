#include "Level.h"
#include <sstream>

// Convenient defines for polymap conversion
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

Level::Level()
{
}


Level::~Level()
{
}

Level::Level(std::string path)
{
	LoadTextures();
	LoadLevel(path);
	ConvertTileMapToPolyMap(0, 0, m_mapWidth, m_mapHeight, m_cellSize, m_mapWidth);
	
	// m_sprites.insert(std::pair<SpriteId, olc::Sprite*>(SpriteId::ROAD, new olc::Sprite("resources/road.png")));
	// m_sprites.insert(std::pair<SpriteId, olc::Sprite*>(SpriteId::BUILDING, new olc::Sprite("resources/building.png")));
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
	if (!(iss >> m_mapWidth >> m_mapHeight >> m_cellSize))
	{
		std::cout << "error reading file";
		return false;
	};
	std::cout << line;
	m_map = new Cell[m_mapWidth * m_mapHeight];

	// read following lines char by char
	char data;
	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)

		{
			if (!input.eof())
			{
				input >> data;
				switch (data)
				{
				case 'C':
				case 'c':
					m_map[x + y * m_mapWidth].obstacle = true;
					m_map[x + y * m_mapWidth].sprId = SpriteId::BUILDING;
					break;
				default:
					m_map[x + y * m_mapWidth].obstacle = false;
					m_map[x + y * m_mapWidth].sprId = SpriteId::ROAD;

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
		return false;

	m_sprites[(size_t)SpriteId::BUILDING] = new olc::Sprite("resources/building.png");
	if (m_sprites[(size_t)SpriteId::BUILDING] == nullptr)
		return false;

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
			if (m_map[i].obstacle)
			{
				// If this cell has no western neighbour, it needs a western edge
				if (!m_map[w].obstacle)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (m_map[n].edge_exist[WEST])
					{
						// Northern neighbour has a western edge, so grow it downwards
						vec_edges[m_map[n].edge_id[WEST]].end.y += fBlockWidth;
						m_map[i].edge_id[WEST] = m_map[n].edge_id[WEST];
						m_map[i].edge_exist[WEST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x; edge.end.y = edge.start.y + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[WEST] = edge_id;
						m_map[i].edge_exist[WEST] = true;
					}
				}

				// If this cell dont have an eastern neignbour, It needs a eastern edge
				if (!m_map[e].obstacle)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (m_map[n].edge_exist[EAST])
					{
						// Northern neighbour has one, so grow it downwards
						vec_edges[m_map[n].edge_id[EAST]].end.y += fBlockWidth;
						m_map[i].edge_id[EAST] = m_map[n].edge_id[EAST];
						m_map[i].edge_exist[EAST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.start.x = (sx + x + 1) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x; edge.end.y = edge.start.y + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[EAST] = edge_id;
						m_map[i].edge_exist[EAST] = true;
					}
				}

				// If this cell doesnt have a northern neignbour, It needs a northern edge
				if (!m_map[n].obstacle)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (m_map[w].edge_exist[NORTH])
					{
						// Western neighbour has one, so grow it eastwards
						vec_edges[m_map[w].edge_id[NORTH]].end.x += fBlockWidth;
						m_map[i].edge_id[NORTH] = m_map[w].edge_id[NORTH];
						m_map[i].edge_exist[NORTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so create one
						Edge edge;
						edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y) * fBlockWidth;
						edge.end.x = edge.start.x + fBlockWidth; edge.end.y = edge.start.y;

						// Add edge to Polygon Pool
						int edge_id = vec_edges.size();
						vec_edges.push_back(edge);

						// Update tile information with edge information
						m_map[i].edge_id[NORTH] = edge_id;
						m_map[i].edge_exist[NORTH] = true;
					}
				}

				// If this cell doesnt have a southern neignbour, It needs a southern edge
				if (!m_map[s].obstacle)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (m_map[w].edge_exist[SOUTH])
					{
						// Western neighbour has one, so grow it eastwards
						vec_edges[m_map[w].edge_id[SOUTH]].end.x += fBlockWidth;
						m_map[i].edge_id[SOUTH] = m_map[w].edge_id[SOUTH];
						m_map[i].edge_exist[SOUTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so I need to create one
						Edge edge;
						edge.start.x = (sx + x) * fBlockWidth; edge.start.y = (sy + y + 1) * fBlockWidth;
						edge.end.x = edge.start.x + fBlockWidth; edge.end.y = edge.start.y;

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

void Level::DrawPolyMap(olc::PixelGameEngine & engine)
{
	for (auto e : vec_edges)
	{
		engine.DrawLine(e.start.x, e.start.y, e.end.x, e.end.y, olc::RED);
		engine.FillCircle(e.start.x, e.start.y, 3, olc::RED);
		engine.FillCircle(e.end.x, e.end.y, 3, olc::RED);

	}
}

void Level::DrawLevel(olc::PixelGameEngine & engine)
{
	// Draw Blocks from TileMap
	for (int x = 0; x < m_mapWidth; x++)
		for (int y = 0; y < m_mapHeight; y++)
		{

			engine.DrawSprite(x * m_cellSize, y * m_cellSize,
				m_sprites[(size_t)m_map[y * m_mapWidth + x].sprId]);
			
		}
}

