#include "ZombieSquad.h"
#include "ZombieHandler.h"
#include "GlobalConstants.h"
#include <algorithm>
#include <functional>

ZombieSquad::ZombieSquad() 
	: m_playerHandler(*this)
{
	sAppName = "ZombieSquad";
}

bool ZombieSquad::OnUserCreate()
{
	// Load Level data
	/*
		world width, height, blockwidth
		start location
		goal location
	*/

	m_currentLevel = new Level("level4.txt");

	// Generate Edge data from tile map
	/*
		see lineOfSight demo
	*/

	// Put player characters into the game
	/*
		Set xy-coordinates
		Set size
		Give control to player
	*/

	// Player 1
	Player* player1 = new Player(m_currentLevel->GetStart().x, m_currentLevel->GetStart().y, 0.0f, *this, true);
	vecActors.push_back(player1);
	m_playerHandler.addPlayer(player1, 0);
	// Player 2
	Player* player2 = new Player(m_currentLevel->GetStart().x + 2.0f, m_currentLevel->GetStart().y, 0.0f, *this);
	vecActors.push_back(player2);
	m_playerHandler.addPlayer(player2, 1);
	// Player 3
	Player* player3 = new Player(m_currentLevel->GetStart().x, m_currentLevel->GetStart().y + 2.0f, 0.0f, *this);
	vecActors.push_back(player3);
	m_playerHandler.addPlayer(player3, 2);

	// Initialize the ZombieHandler
	m_zombieHandler.Init(*player1, *player2, *player3);

	// Populate Level with zombies
	/*
		For each open cell far enough away from start
		- Add zombies to unoccupied cells
	*/

	for (int y = 0; y < m_currentLevel->GetNumCellsY(); y++)
	{
		for (int x = 0; x < m_currentLevel->GetNumCellsX(); x++)
		{
			int distance = ManhattanDistance(m_currentLevel->GetStartX(), m_currentLevel->GetStartY(), x, y);
			if (distance > MINIMUM_DISTANCE && m_currentLevel->GetCell(x, y)->obstacle == false)
			{
				int numZombies = rand() % 2;
				for (int i = 0; i <= numZombies; i++)
				{
					SpawnZombie(x, y, i * 0.5f);
				}
			}
		}
	}
	return true;
}

bool ZombieSquad::OnUserUpdate(float fElapsedTime)
{
	DoInput(fElapsedTime);
	// Check input

	DoUpdate(fElapsedTime);
	// Update 
	/*
		Check victory condition
		- All player characters are in the goal cell
		Check Collisions
		Resolve collisions
		- Move zombies and characters away from walls
		- destroy bullets, when they touch something
		Move Actors (zombies, characters, bullets)
		Calculate visibility polygons

	*/

	/*
		Draw level, and actors.
	*/
	DoDraw();

	return m_isRunning;
}

void ZombieSquad::DoInput(float fElapsedTime)
{
	/*
	Check player inputs
	- Keyboard:
	- W/S - Move forward/back
	- A/D - Turn left/right
	- 1,2,3,4 - switch characters
	- Space - fire gun -> create bullets
	*/

	m_playerHandler.HandlePlayers(fElapsedTime);
	// m_bulletHandler.HandleBullets(fElapsedTime);
	m_zombieHandler.HandleZombies(fElapsedTime);


	if (GetKey(olc::ESCAPE).bReleased)
	{
		m_isRunning = false;
	}

}

void ZombieSquad::DoUpdate(float fElapsedTime)
{
	/* REMOVED: until can check vectors of Player-pointers
	if (m_currentLevel->CheckVictory(m_selectedPlayer))
	{
		// DrawString(1, 1, "You Win!", olc::WHITE, 2U);
		std::cout << "You Win! Yattaa!\n";
	}
	*/

	// update actors
	for (auto& itr : vecActors)
	{
		itr->Update(fElapsedTime);
	}

	std::vector<CECollision> vec_circleEdgeColliders; // Container to hold colliding actors and edges
	std::vector<CCCollision> vec_circleCircleColliders; // Container to hold colliding actors and edges

	for (auto& itr : vecActors)
	{

		for (auto& edge : m_currentLevel->GetEdges())
		{
			float overlap = m_physics.isColliding(*itr, edge);
			if (overlap > 0.0f)
			{
				vec_circleEdgeColliders.emplace_back(itr, edge.normal, overlap);
			}

		}
		// check collision with other actors
		for (auto& other : vecActors)
		{
			// avoid self checking
			if (itr->m_id == other->m_id)
			{
				continue;
			}
			float overlap = m_physics.isColliding(*itr, *other);
			if (overlap > 0.0f)
			{
				// actors are colliding
				vec_circleCircleColliders.emplace_back(itr, other, overlap);
			}
		}

	}

	// For each item in CollidingActors- vector container, resolve collisions
	for (auto p : vec_circleEdgeColliders)
	{
		m_physics.resolveEdgeCircle(p.actor, p.normal, p.distance);
	}
	// for each item in circle-circle collisions container, resolve collision
	for (auto p : vec_circleCircleColliders)
	{
		m_physics.resolveCircleCircle(p.lhs, p.rhs, p.overlap);
	}

	vec_circleEdgeColliders.clear();
	vec_circleCircleColliders.clear();



	// DELETE ACTORS
	m_zombieHandler.RemoveDestroyed();

	// Remove destroyed actors
	// https://stackoverflow.com/questions/991335/how-to-erase-delete-pointers-to-objects-stored-in-a-vector rlbond's answer
	// Apply a deleting function to each element. (call the destructor and make the pointer null)
	std::for_each(vecActors.begin(), vecActors.end(), [](Actor*& e)
		{
			if (e->GetDestroyed())
			{
				delete e;
				e = nullptr;
			}
		});
	// remove re-orders the vector, so removed elements are at the back of the container
	std::vector<Actor*>::iterator newEnd = std::remove(vecActors.begin(), vecActors.end(), static_cast<Actor*>(nullptr));
	// erase the elements from the vector
	vecActors.erase(newEnd, vecActors.end());
	// DELETE ACTORS END
}

void ZombieSquad::DoDraw()
{
	// Drawing
	/*
		Clear the screen

		Draw tiles
		Draw visibility
		Draw Actors (if in polygon)
	*/
	Clear(olc::BLACK);

	m_currentLevel->DrawLevel(*this);
	m_currentLevel->DrawPolyMap(*this);

	for (auto& itr : vecActors)
	{
		itr->Draw(*this);
	}
}

void ZombieSquad::SpawnZombie(int x, int y, float offset)
{
	float size = m_currentLevel->GetCellSize();
	float xPos = (x * size) + (size / 2.0f) + offset;
	float yPos = (y * size) + (size / 2.0f);
	Zombie* zomb = new Zombie(xPos, yPos, *this);
	vecActors.push_back(zomb);
	m_zombieHandler.AddZombie(zomb);
}

void ZombieSquad::SpawnBullet(const Vec2f& pos, float dir, float lifetime, float speed, ActorTag tag)
{
	vecActors.push_back(new Bullet(pos, dir, lifetime, speed, tag));
}