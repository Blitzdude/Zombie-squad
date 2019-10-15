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
	std::cout << ScreenWidth() << " , " << ScreenHeight() << "\n";

	bool createSuccess = false;

	// Load Level data
	m_currentLevel = new Level("level4.txt", (float)ScreenWidth(), (float)ScreenHeight());
	m_currentLevel->InitPathfinding();

	float startX = m_currentLevel->GetStart().x;
	float startY = m_currentLevel->GetStart().y;

	// Put player characters into the game
	// Player 1
	Player* player1 = SpawnPlayer(startX, startY, 0.0f, 0, *this, m_playerHandler, 0.0f, true);
	// Player 2
	Player* player2 = SpawnPlayer(startX, startY, 0.0f, 1, *this, m_playerHandler, 3.0f);
	// Player 3
	Player* player3 = SpawnPlayer(startX, startY, 0.0f, 2, *this, m_playerHandler, 6.0f);

	// Initialize handlers
	createSuccess = m_zombieHandler.Init(*player1, *player2, *player3);
	createSuccess = m_playerHandler.Init(m_zombieHandler);

	// Populate Level with zombies
	for (int y = 0; y < m_currentLevel->GetNumCellsY(); y++)
	{
		for (int x = 0; x < m_currentLevel->GetNumCellsX(); x++)
		{
			int distance = ManhattanDistance(m_currentLevel->GetStartX(), m_currentLevel->GetStartY(), x, y);
			if (distance > MINIMUM_DISTANCE && m_currentLevel->GetCell(x, y)->isObstacle == false)
			{
				int numZombies = ZOMBIES_MAX_NUM_PER_TILE - 1; // rand() % 2;
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
	
	// If createSuccess is false, Initializing handlers failed
	return createSuccess;
}

bool ZombieSquad::OnUserUpdate(float fElapsedTime)
{
	// input outside so we can quit
	DoInput(fElapsedTime);
	
	if (!m_isGameOver)
	{
		DoUpdate(fElapsedTime);

		// Draw level, and actors. 
		DoDraw();
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

	if (m_isRunning)
	{
		m_playerHandler.HandlePlayers(fElapsedTime);;
		m_zombieHandler.HandleZombies(fElapsedTime);
	}

	if (GetKey(olc::ESCAPE).bReleased)
	{
		m_isRunning = false;
	}

}

void ZombieSquad::DoUpdate(float fElapsedTime)
{
	// checkGame conditions
	m_isGameOver = GameIsOver();

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
	// m_currentLevel->DrawPolyMap(*this);
	// m_currentLevel->DrawConnections(*this);
	for (auto& itr : vecActors)
	{
		itr->Draw(*this);
	}

	if (m_isRunning && !m_isGameOver)
	{
		// Draw instruction text
		const int c_textXPos = 2;
		const int c_textYPos = ScreenHeight() - 46;
		const std::string c_instructionText =
			"Get to the safe house and try not to die. Save as many survivors as you can"
			"1 2 3-keys: Change survivors\n"
			"W/A/S/D-keys: Move forward/back. Turn left/right\n"
			"Space: Shoot\n"
			"Esc-key: Exit game\n";
		
		DrawString(c_textXPos, c_textYPos, c_instructionText);
	}
	else if (m_isRunning && m_isGameOver)
	{
		// game is over
		int numAlive = m_playerHandler.NumberOfPlayersAlive();
		if (numAlive <= 0)
		{
			const std::string c_deathText =
				"All the survivors are dead.\n"
				"Care to try again?\n";

#pragma warning (disable : 4244) // narrowing conversion from float to int32_t
			DrawString(ScreenWidth() / 6.0f, ScreenHeight() / 4.0f, c_deathText, olc::RED, 4U);
#pragma warning (default : 4244)
		}
		else 
		{
			// At least one survivor is alive.
			const std::string c_victoryText1 =
				"Congratulations!\n"
				"You have successfully guided \n";

			const std::string c_victoryText2 =
				" of your survivors to the safe house!\n"
				"Thank you for playing my game!\n";

			std::string victoryTextWhole = c_victoryText1 + std::to_string(numAlive) + c_victoryText2;
#pragma warning (disable : 4244) // narrowing conversion from float to int32_t
			DrawString(ScreenWidth() / 6.0f, ScreenHeight() / 4.0f, victoryTextWhole, olc::RED, 3U);
#pragma warning (default : 4244)
		}
	}
	
}

bool ZombieSquad::CheckVictory()
{
	if (m_playerHandler.NumberOfPlayersAlive() > 0)
	{
		// someone is alive. You win!
		return true;
	}
	// everyone is dead.
	return false;
}
bool ZombieSquad::GameIsOver()
{
	if (m_playerHandler.NumberOfPlayersAlive() <= 0 ||
		m_playerHandler.NumberOfPlayersOnGoal() >= m_playerHandler.NumberOfPlayersAlive())
	{
		// all players are dead or on the goal tile
		return true;
	}
	return false;
}

Player* ZombieSquad::SpawnPlayer(float xPos, float yPos, float dir, int playerNum, 
	ZombieSquad& game, PlayerHandler& playerHandler, float offset, bool startingPlayer)
{
	float size = m_currentLevel->GetCellSize();
	// float xPos = (x * size) + (size / 2.0f) + offset;
	// float yPos = (y * size) + (size / 2.0f);
	Player* player = new Player(xPos + offset, yPos, dir, playerNum + 1, game, playerHandler, startingPlayer);
	// add player's directly to game, because it's the first thing we do.
	m_playerHandler.addPlayer(player, playerNum);
	vecActors.push_back(player);
	return player;
}

void ZombieSquad::SpawnZombie(int xCell, int yCell, float offset)
{
	float size = m_currentLevel->GetCellSize();
	Vec2f center = Level::GetCellCenterPos(xCell, yCell);
	
	float xPos = center.x + offset;
	float yPos = center.y;
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
