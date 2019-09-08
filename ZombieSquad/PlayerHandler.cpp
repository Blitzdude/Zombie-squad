#include "PlayerHandler.h"
#include "ZombieSquad.h"
#include "Command.h"
#include "Zombie.h"
#include "GlobalConstants.h"
#include "Player.h"
#include "Physics.h"
#include <assert.h>
#include <limits>

PlayerHandler::PlayerHandler(ZombieSquad& engine)
	: m_game(&engine), m_selectedPlayer(nullptr)
{
	bindButtons();
}

PlayerHandler::~PlayerHandler()
{
	// Cleaning, because we are nice
	delete buttonW; buttonW = nullptr;
	delete buttonA; buttonA = nullptr;
	delete buttonS; buttonS = nullptr;
	delete buttonD; buttonD = nullptr;
	delete buttonSpace; buttonSpace = nullptr;
	delete buttonChangeFalse; buttonChangeFalse = nullptr;
	delete buttonChangeTrue; buttonChangeTrue = nullptr;
}

bool PlayerHandler::Init(ZombieHandler& zombieHandler)
{
	m_zombieHandler = &zombieHandler;
	if (m_zombieHandler == nullptr)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

Command * PlayerHandler::handleInput()
{
	if (m_game->GetKey(olc::W).bHeld) return buttonW;
	if (m_game->GetKey(olc::A).bHeld) return buttonA;
	if (m_game->GetKey(olc::S).bHeld) return buttonS;
	if (m_game->GetKey(olc::D).bHeld) return buttonD;
	if (m_game->GetKey(olc::SPACE).bReleased) return buttonSpace;

	if (m_game->GetKey(olc::K1).bReleased)
	{
		buttonChangeFalse->execute(*m_selectedPlayer, 0.0f);
		m_selectedPlayer = m_players[0];
		buttonChangeTrue->execute(*m_selectedPlayer, 0.0f);
		return nullptr;
	}
	else if (m_game->GetKey(olc::K2).bReleased)
	{
		buttonChangeFalse->execute(*m_selectedPlayer, 0.0f);
		m_selectedPlayer = m_players[1];
		buttonChangeTrue->execute(*m_selectedPlayer, 0.0f);

		return nullptr;
	}
	else if (m_game->GetKey(olc::K3).bReleased)
	{
		buttonChangeFalse->execute(*m_selectedPlayer, 0.0f);
		m_selectedPlayer = m_players[2];
		buttonChangeTrue->execute(*m_selectedPlayer, 0.0f);
		return nullptr;
	}

	return nullptr;
}

void PlayerHandler::HandlePlayers(float fElapsedTime)
{
	// New player handling
	// get input 
	// go through the plaeyrs
	// if the player has taken damage -> command it to die
	// if player state is controlled and command is not nullptr
	// else if player state is overwatch
	// check if zombies are close and fire at them
	Command* command = handleInput();
	for (auto& p : m_players)
	{
		if (p->GetIsHit())
		{
			// create a new command to kill the player character
			Command* youMustDie = new Die();
			youMustDie->execute(*p, fElapsedTime);
		}
		else if (command && p->GetCurrentState()->GetStateID() == StateID::PLAYER_CONTROLLED)
		{
			command->execute(*p, fElapsedTime);
		}
		else if (p->GetCurrentState()->GetStateID() == StateID::PLAYER_OVERWATCH)
		{
			// check if zombies are in front of the player and in range
			// GetClosestZombie()
		}
	}
	
}

void PlayerHandler::bindButtons()
{
	buttonW = new MoveForward();
	buttonA = new TurnLeft();
	buttonS = new MoveBack();
	buttonD = new TurnRight();

	buttonChangeTrue = new ChangePlayer(true);
	buttonChangeFalse = new ChangePlayer(false);
	// buttonKey1 = new ChangePlayer(); // TODO: We only need one
	// buttonKey2 = new ChangePlayer();
	// buttonKey3 = new ChangePlayer();

	buttonSpace = new Attack();
}

void PlayerHandler::addPlayer(Player* player, int index)
{
	assert(index < 3);
	m_players[index] = player;
	if (m_selectedPlayer == nullptr)
	{
		m_selectedPlayer = m_players[index];
	}
}

// returns nullptr if no zombie was found
const Zombie* PlayerHandler::GetClosestVisibleZombiePosition(const Player& player)
{
	float distance = std::numeric_limits<float>::infinity();
	Zombie* ret = nullptr;
	for (auto zomb : m_zombieHandler->GetZombies())
	{
		// if player sees target check it's distance, for closest;
		if (PlayerSeesTarget(zomb->GetPosition(), player))
		{
			float compare = Vec2f::DistanceBetween(player.GetPosition(), zomb->GetPosition());
			if (compare < distance)
			{
				ret = zomb;
				distance = compare;
			}
		}
	}
	// if ret is nullptr, no zombie was found
	return ret;
}

bool PlayerHandler::PlayerSeesTarget(const Vec2f& targetPos, const Player& player)
{
	// is the target close enough
	if (Vec2f::DistanceBetween(targetPos, player.GetPosition()) <= ZOMBIE_SIGHT_RANGE)
	{
		// get the directional vectors left and right
		Vec2f left = player.GetDirectionVector().GetRotated(ZOMBIE_SIGHT_FOV_RAD) + player.GetPosition();
		Vec2f right = player.GetDirectionVector().GetRotated(-ZOMBIE_SIGHT_FOV_RAD) + player.GetPosition();
		// if target is left-side of right and right-side of left, target is in the cone
		if (!Vec2f::IsLeft(player.GetPosition(), left, targetPos) &&
			Vec2f::IsLeft(player.GetPosition(), right, targetPos))
		{
			// if target can be hit with a ray, it is visible
			Ray ray(player.GetPosition(), targetPos);
			for (auto edge : m_game->m_currentLevel->GetEdges())
			{
				if (Physics::CheckLineIntersection(ray, edge))
				{
					return false;
				}
			}
			// if no intersections were found, zombie sees the target
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

