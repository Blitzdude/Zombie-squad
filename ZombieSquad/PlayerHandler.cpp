#include "PlayerHandler.h"
#include "Command.h"
#include <assert.h>

PlayerHandler::PlayerHandler(olc::PixelGameEngine & engine)
	: m_engine(&engine), m_selectedPlayer(nullptr)
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
}

Command * PlayerHandler::handleInput()
{
	if (m_engine->GetKey(olc::W).bHeld) return buttonW;
	if (m_engine->GetKey(olc::A).bHeld) return buttonA;
	if (m_engine->GetKey(olc::S).bHeld) return buttonS;
	if (m_engine->GetKey(olc::D).bHeld) return buttonD;

	if (m_engine->GetKey(olc::K1).bReleased)
	{
		m_selectedPlayer = m_players[0];
		return buttonKey1;
	}
	if (m_engine->GetKey(olc::K2).bReleased)
	{
		m_selectedPlayer = m_players[1];
		return buttonKey2;
	}
	if (m_engine->GetKey(olc::K3).bReleased)
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
}

void PlayerHandler::bindButtons()
{
	buttonW = new MoveForward();
	buttonA = new TurnLeft();
	buttonS = new MoveBack();
	buttonD = new TurnRight();

	buttonKey1 = new ChangePlayer(); // TODO: We only need one
	buttonKey2 = new ChangePlayer();
	buttonKey3 = new ChangePlayer();
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
