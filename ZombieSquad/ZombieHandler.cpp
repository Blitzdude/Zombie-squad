#include "ZombieHandler.h"

ZombieHandler::ZombieHandler(const Player& p1, const Player& p2, const Player& p3)
{
}

ZombieHandler::~ZombieHandler()
{
	// We are only clearing, because we are not responsible for players
	m_player1 = nullptr;
	m_player2 = nullptr;
	m_player3 = nullptr;
}

Command* ZombieHandler::handleInput(Zombie& actor)
{
	// Check if a zombie is close enough to a player
	// if is -> command the zombie to chase it
	return nullptr;
}
