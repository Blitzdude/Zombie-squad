#include "PlayerHandler.h"
#include "Command.h"

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

	if (m_engine->GetKey(olc::K1).bReleased) return new ChangePlayer(1);
	if (m_engine->GetKey(olc::K2).bReleased) return new ChangePlayer(2);
	if (m_engine->GetKey(olc::K3).bReleased) return new ChangePlayer(3);


	return nullptr;
}

void PlayerHandler::bindButtons()
{
	buttonW = new MoveForward();
	buttonA = new TurnLeft();
	buttonS = new MoveBack();
	buttonD = new TurnRight();
}

void PlayerHandler::addPlayer(Player* player)
{
	m_players.push_back(player);
	if (m_selectedPlayer == nullptr)
	{
		m_selectedPlayer = m_players.back();
	}
}
