#include "PlayerHandler.h"
#include "Command.h"

PlayerHandler::PlayerHandler(olc::PixelGameEngine & engine)
	: m_engine(&engine)
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

	return nullptr;
}

void PlayerHandler::bindButtons()
{
	buttonW = new MoveForward();
	buttonA = new TurnLeft();
	buttonS = new MoveBack();
	buttonD = new TurnRight();
}
