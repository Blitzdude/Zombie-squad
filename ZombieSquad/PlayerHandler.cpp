#include "PlayerHandler.h"
#include "ZombieSquad.h"
#include "Command.h"
#include "Player.h"
#include <assert.h>

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
		m_selectedPlayer = m_players[0];
		return buttonKey1;
	}
	else if (m_game->GetKey(olc::K2).bReleased)
	{
		m_selectedPlayer = m_players[1];
		return buttonKey2;
	}
	else if (m_game->GetKey(olc::K3).bReleased)
	{
		m_selectedPlayer = m_players[2];
		return buttonKey3;
	}

	return nullptr;
}

void PlayerHandler::HandlePlayers(float fElapsedTime)
{
	
	Command* command = handleInput();
	if (command)
	{
		command->execute(*m_selectedPlayer, fElapsedTime);
	}

	// command the uncontrolled players
	for (auto& p : m_players)
	{
		// Check if player has taken damage
		if (p->GetIsHit())
		{
			Command* c = new Die();
			c->execute(*p, fElapsedTime);
		}
		// If the state is overwatch, find the closest zombie
		// Vec2f target = GetClosestZombiePosition(p->GetPosition()); // TODO:: what is wanted
		// tell player to aim it's way
	}
}

void PlayerHandler::bindButtons()
{
	buttonW = new MoveForward();
	buttonA = new TurnLeft();
	buttonS = new MoveBack();
	buttonD = new TurnRight();

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
