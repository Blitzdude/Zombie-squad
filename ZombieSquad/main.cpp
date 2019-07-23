#define OLC_PGE_APPLICATION
#include "ZombieSquad.h"
#include <vector>
#include <iostream>

/*
ZombieSquad - Project by Joel Känsälä. Public license.
*/

/* STYLE GUIDE
types: (struct, class etc)
	- TypeName
methods, functions: 
	- FunctionName()
variables:
	- type_variableNameAbc
	- type : tells which type the variable is
	- m_=member, c_=constants, p_=parameter
multiple types:
	- mc_= member constant etc.
global constants/Defines:
	- ALL_UPPER_CASE
	
*/

// CLASSES
/*
Level
	- holds all data for the map
	- Tile data
	- Edge data
	- Handles pathfindng from one point to another (A* star)
*/

/*
Command
	- part of command pattern
	- command object given to actors
	- execute()
*/

/*
Actors::Player, Zombie, Bullet
	- position
	- direction
	- speed
	- attack()
	- move()
	- have states
*/

/*
State
	- current state
	- what actor is currently doing
	- has entry, current and leave actions
*/

/*
Handler::Input, ZombieAI, CharacterAI
	- Input: send commands based on input.
	- ZombieAI: tell zombies to do stuff
	- CharacterAI: tell characters not beign controlled to do stuff
*/

/*
Physics
	- list of bodies (if necessary)
	- collision detection
	- collision resolution
		- circle vs circle
		- circle vs edge
	- raycasting
		- raycast result struct
*/
// OTHER
/*
	Math
	- utility 
	- vec2
*/

//
//class ZombieSquad : public olc::PixelGameEngine
//{
//
//
//public:
//	ZombieSquad()
//		: m_playerHandler(*this)
//	{
//		sAppName = "ZombieSquad";
//	}
//
//	bool OnUserCreate() override
//	{
//		// Load Level data
//		/*
//			world width, height, blockwidth
//			start location
//			goal location
//		*/
//
//		m_currentLevel = new Level("level4.txt");
//
//		// Generate Edge data from tile map
//		/*
//			see lineOfSight demo
//		*/
//
//		// Put player characters into the game
//		/*
//			Set xy-coordinates
//			Set size
//			Give control to player
//		*/
//		// Player 1
//		Player* player1 = new Player(m_currentLevel->GetStart().x, m_currentLevel->GetStart().y);
//		vecActors.push_back(player1);
//		m_playerHandler.addPlayer(player1, 0);
//		// Player 2
//		Player* player2 = new Player(m_currentLevel->GetStart().x + 2.0f, m_currentLevel->GetStart().y);
//		vecActors.push_back(player2);
//		m_playerHandler.addPlayer(player2, 1);
//		// Player 3
//		Player* player3 = new Player(m_currentLevel->GetStart().x, m_currentLevel->GetStart().y + 2.0f);
//		vecActors.push_back(player3);
//		m_playerHandler.addPlayer(player3, 2);
//
//		const int MINIMUM_DISTANCE = 5;
//
//		// For each cell, see if it's far enough from start and spawn zombies there
//		for (int y = 0; y < m_currentLevel->GetNumCellsY(); y++)
//		{
//			for (int x = 0; x < m_currentLevel->GetNumCellsX(); x++)
//			{
//				int distance = ManhattanDistance(m_currentLevel->GetStartX(), m_currentLevel->GetStartY(), x, y);
//				if ( distance > MINIMUM_DISTANCE && m_currentLevel->GetCell(x, y)->obstacle == false )
//				{
//					int numZombies = rand() % 3;
//					for (int i = 0; i <= numZombies; i++)
//					{
//						SpawnZombie(x, y, i * 0.5f);
//					}
//				}
//			}
//		}
//		
//
//
//		// Populate Level with zombies
//		/*
//			For each open cell far enough away from start
//			- Add zombies to unoccupied cells
//		*/
//
//		return true;
//	}
//
//	bool OnUserUpdate(float fElapsedTime) override
//	{
//		DoInput(fElapsedTime);
//		// Check input
//		
//
//		DoUpdate(fElapsedTime);
//		// Update 
//		/*
//			Check victory condition
//			- All player characters are in the goal cell
//			Check Collisions
//			Resolve collisions
//			- Move zombies and characters away from walls
//			- destroy bullets, when they touch something
//			Move Actors (zombies, characters, bullets)
//			Calculate visibility polygons
//			
//		*/
//
//		DoDraw();
//		
//		return m_isRunning;
//	}
//
//	void DoInput(float fElapsedTime)
//	{
//		/*
//		Check player inputs
//		- Keyboard:
//		- W/S - Move forward/back
//		- A/D - Turn left/right
//		- 1,2,3,4 - switch characters
//		- Space - fire gun -> create bullets
//		*/
//		
//		// TODO: move player command execution to inside player handler
//
//		m_playerHandler.HandlePlayers(fElapsedTime);
//		// m_bulletHandler.HandleBullets(fElapsedTime);
//		// m_zombieHandler.HandleZombies(fElapsedTime);
//
//		/*
//		Command* command = m_playerHandler.handleInput();
//		if (command)
//		{
//			command->execute(*m_selectedPlayer, fElapsedTime);
//		}
//		*/
//
//		if (GetKey(olc::ESCAPE).bReleased)
//		{
//			m_isRunning = false;
//		}
//
//	}
//
//	void DoUpdate(float fElapsedTime)
//	{
//		/* REMOVED: until can check vectors of Player-pointers
//		if (m_currentLevel->CheckVictory(m_selectedPlayer))
//		{
//			// DrawString(1, 1, "You Win!", olc::WHITE, 2U);
//			std::cout << "You Win! Yattaa!\n";
//		}
//		*/
//
//		std::vector<CECollision> vec_circleEdgeColliders; // Container to hold colliding actors and edges
//		std::vector<CCCollision> vec_circleCircleColliders; // Container to hold colliding actors and edges
//
//		for (auto& itr : vecActors)
//		{
//			
//			for (auto& edge : m_currentLevel->GetEdges())
//			{
//				float overlap = m_physics.isColliding(*itr, edge);
//				if (overlap > 0.0f)
//				{
//					vec_circleEdgeColliders.emplace_back(itr, edge.normal, overlap);
//				}
//
//			}
//			// check collision with other actors
//			for (auto& other : vecActors)
//			{
//				// avoid self checking
//				if (itr->m_id == other->m_id)
//				{
//					continue;
//				}
//				float overlap = m_physics.isColliding(*itr, *other);
//				if (overlap > 0.0f)
//				{
//					// actors are colliding
//					vec_circleCircleColliders.emplace_back(itr, other, overlap);
//				}
//			}
//
//		}
//
//		// For each item in CollidingActors- vector container, resolve collisions
//		for (auto p : vec_circleEdgeColliders)
//		{
//			m_physics.resolveEdgeCircle(p.actor, p.normal, p.distance);
//		}
//		// for each item in circle-circle collisions container, resolve collision
//		for (auto p : vec_circleCircleColliders)
//		{
//			m_physics.resolveCircleCircle(p.lhs, p.rhs, p.overlap);
//		}
//
//		vec_circleEdgeColliders.clear();
//		vec_circleCircleColliders.clear();
//	}
//
//	void DoDraw()
//	{
//		// Drawing
//		/*
//			Clear the screen
//
//			Draw tiles
//			Draw visibility
//			Draw Actors (if in polygon)
//		*/
//		Clear(olc::BLACK);
//
//		m_currentLevel->DrawLevel(*this);
//		m_currentLevel->DrawPolyMap(*this);
//
//		for (auto & itr : vecActors)
//		{
//			itr->Draw(*this);
//		}
//	}
//
//	void SpawnZombie(int x, int y, float offset = 0.0f)
//	{
//		float size = m_currentLevel->GetCellSize();
//		float xPos = (x * size) + (size / 2.0f) + offset;
//		float yPos = (y * size) + (size / 2.0f);
//		vecActors.push_back(new Zombie(xPos, yPos));
//	}
//
//	void SpawnBullet(const Vec2f& pos, float dir, float lifetime)
//	{
//		const float BULLET_SIZE = 5.0f;
//		vecActors.push_back(new Bullet(pos, dir, lifetime));
//	}
//
//private:
//	PlayerHandler m_playerHandler;
//	// std::vector<Actor*> m_players; // Moved to player handler
//	// Actor* m_selectedPlayer; // Moved to player handler
//	std::vector<Actor*> vecActors;
//	Level* m_currentLevel;
//	Physics m_physics;
//
//	bool m_isRunning = true;
//};


int main()
{
	ZombieSquad game;
	if (game.Construct(800, 600, 1, 1))
	{
		game.Start();
	}
	return 0;
}