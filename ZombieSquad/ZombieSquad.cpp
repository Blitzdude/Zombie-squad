#include "ZombieSquad.h"
#include "GlobalConstants.h"
#include "Physics.h"
#include <algorithm>
#include <functional>

ZombieSquad::ZombieSquad() 
	: m_playerHandler(*this), m_zombieHandler(*this)
{
	sAppName = "ZombieSquad";
}

bool ZombieSquad::OnUserCreate()
{
	bool createSuccess = false;
	// Load Level data
	/*
		world width, height, blockwidth
		start location
		goal location
	*/

	m_currentLevel = new Level("level4.txt");
	m_currentLevel->InitPathfinding();
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

	float startX = m_currentLevel->GetStart().x;
	float startY = m_currentLevel->GetStart().y;

	// Player 1
	Player* player1 = SpawnPlayer(startX, startY, 0.0f, 0, *this, m_playerHandler, 0.0f, true);

	// Player 2
	Player* player2 = SpawnPlayer(startX, startY, 0.0f, 1, *this, m_playerHandler, 3.0f);

	// Player 3
	Player* player3 = SpawnPlayer(startX, startY, 0.0f, 2, *this, m_playerHandler, 6.0f);


	// Initialize the ZombieHandler
	createSuccess = m_zombieHandler.Init(*player1, *player2, *player3);
	createSuccess = m_playerHandler.Init(m_zombieHandler);
	player1 = nullptr;
	player2 = nullptr;
	player3 = nullptr;

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
	// initialize flags
	m_isRunning = true;
	m_isGameOver = false;
	m_isWin = false;
	
	vecDebugPath = m_currentLevel->GetPathToTarget(m_currentLevel->GetStart(), m_currentLevel->GetEnd());

	// If createSuccess is false, Initializing handlers failed
	return createSuccess;
}

bool ZombieSquad::OnUserUpdate(float fElapsedTime)
{
	if (!m_isGameOver)
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
	}
	if (m_isGameOver && !m_isWin)
	{
		// all players are dead
		std::cout << "You and your friends are dead. Game Over\n";
	}
	else if (m_isGameOver && m_isWin)
	{
		std::cout << "You have survived, Congratulations!\n";
	}

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

	// checkGame conditions
	m_isGameOver = CheckVictory();

	AddActors();
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
			float overlap = Physics::isColliding(*itr, edge);
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
			float overlap = Physics::isColliding(*itr, *other);
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
		Physics::resolveEdgeCircle(p.actor, p.normal, p.distance);
	}
	// for each item in circle-circle collisions container, resolve collision
	for (auto p : vec_circleCircleColliders)
	{
		Physics::resolveCircleCircle(p.lhs, p.rhs, p.overlap);
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
	// m_currentLevel->DrawConnections(*this);
	for (auto& itr : vecActors)
	{
		itr->Draw(*this);
	}
	

	// DEBUG: Draw the cells with the path
	
	if (!vecDebugPath.empty())
	{
		for (auto itr = vecDebugPath.begin(); itr != vecDebugPath.end(); itr++)
		{
			if (itr != vecDebugPath.begin())
			{

			auto prev = std::prev(itr);
			DrawLine(m_currentLevel->GetCellCenterPos(itr->first, itr->second).x, 
					 m_currentLevel->GetCellCenterPos(itr->first, itr->second).y,
					 m_currentLevel->GetCellCenterPos(prev->first, prev->second).x,
					 m_currentLevel->GetCellCenterPos(prev->first, prev->second).y,
					 olc::RED);
			}
		}
	}
	
}

bool ZombieSquad::CheckVictory()
{
	// returns condition, that tells if game is over
	bool gameOver = true;
	// Check if all players are dead
	for (auto itr : m_playerHandler.GetPlayers())
	{
		if (!itr->GetIsHit())
		{
			// players are alive -> game is not over
			gameOver =  false;
		}
	}
	
	bool win = true;
	// if the player has won -> end the game
	for (auto itr : m_playerHandler.GetPlayers())
	{
		if (!(m_currentLevel->GetCell(itr->GetPosition())->isGoal) && 
			(itr->GetCurrentState()->GetStateID() != StateID::STATE_DEAD) )
		{
			win = false;
		}
	}

	m_isWin = win;
	gameOver = win == true ? true : false;
	// if this gets here, the game is over
	return gameOver;
}

Player* ZombieSquad::SpawnPlayer(float xPos, float yPos, float dir, int playerNum, ZombieSquad& game, PlayerHandler& playerHandler, float offset, bool startingPlayer)
{
	float size = m_currentLevel->GetCellSize();
	// float xPos = (x * size) + (size / 2.0f) + offset;
	// float yPos = (y * size) + (size / 2.0f);
	Player* player = new Player(xPos + offset, yPos, dir, game, playerHandler, startingPlayer);
	// add player's directly to game, because it's the first thing we do.
	m_playerHandler.addPlayer(player, playerNum);
	vecActors.push_back(player);
	return player;
}

void ZombieSquad::SpawnZombie(int xCell, int yCell, float offset)
{
	float size = m_currentLevel->GetCellSize();
	float xPos = (xCell * size) + (size / 2.0f) + offset;
	float yPos = (yCell * size) + (size / 2.0f);
	Zombie* zomb = new Zombie(xPos, yPos, *this, m_zombieHandler);
	vecActorsToAdd.push_back(zomb);
	m_zombieHandler.AddZombie(zomb);
}

void ZombieSquad::SpawnBullet(const Vec2f& pos, float dir, float lifetime, float speed, ActorTag tag)
{
	vecActorsToAdd.push_back(new Bullet(pos, dir, lifetime, speed, tag));
}

void ZombieSquad::AddActors()
{
	// add the add the actors to be added
	for (auto itr : vecActorsToAdd)
	{
		vecActors.push_back(itr);
	}

	vecActorsToAdd.clear();
}
