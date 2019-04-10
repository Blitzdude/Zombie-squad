#include "Level.h"
#include <sstream>


Level::Level()
{
}


Level::~Level()
{
}

Level::Level(std::string path)
{
	LoadLevel(path);
	m_sprites[(size_t)SpriteId::ROAD] = new olc::Sprite("resources/road.png");
	m_sprites[(size_t)SpriteId::BUILDING] = new olc::Sprite("resources/building.png");

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
					m_map[x + y * m_mapWidth].walkable = true;
					m_map[x + y * m_mapWidth].sprId = SpriteId::BUILDING;
					break;
				default:
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

void Level::DrawLevel(olc::PixelGameEngine & engine)
{
	// Draw Blocks from TileMap
	for (int x = 0; x < m_mapWidth; x++)
		for (int y = 0; y < m_mapHeight; y++)
		{

			engine.DrawSprite(x * m_cellSize, y * m_cellSize,
				m_sprites[(size_t)m_map[y * m_mapWidth + x].sprId]);
			/*
			engine.DrawSprite(x * m_cellSize, y * m_cellSize,
				m_sprites.find(m_map[y * m_mapWidth + x].sprId)->second);
			*/


			/*
			if (m_map[y * m_mapWidth + x].walkable)
			{
				//engine.FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::BLUE);
			}
			*/
		}
}

