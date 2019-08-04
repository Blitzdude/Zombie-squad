#include "ZombieHandler.h"
#include "Command.h"

ZombieHandler::ZombieHandler(Player& p1, Player& p2, Player& p3)
	: m_player1(&p1), m_player2(&p2), m_player3(&p3)
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
	static const float SIGHT_RANGE = 200.0f; // variable to control enemy sight range
	
	const Player* player = GetClosestPlayer(actor);
	if (player != nullptr &&
		Vec2f::DistanceBetween(actor.GetPosition(), player->GetPosition()) <= SIGHT_RANGE)
	{
		return new ChasePlayer(player);
		
	}

	return nullptr;
}


const Player* ZombieHandler::GetClosestPlayer(Zombie& zombie)
{
	Player* ret = m_player1;
	Vec2f Zpos = zombie.GetPosition();

	// Check if player2 is closer then player1
	if (Vec2f::DistanceBetween(Zpos, m_player2->GetPosition()) <
		Vec2f::DistanceBetween(Zpos, m_player1->GetPosition()))
	{
		ret = m_player2;
	}

	// Check if player3 is closer then player2
	if (Vec2f::DistanceBetween(Zpos, m_player3->GetPosition()) <
		Vec2f::DistanceBetween(Zpos, m_player2->GetPosition()))
	{
		ret = m_player2;
	}

	return ret;
}
