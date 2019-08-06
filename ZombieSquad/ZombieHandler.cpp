#include "ZombieHandler.h"
#include "Command.h"
#include "GlobalConstants.h"

ZombieHandler::ZombieHandler()
	: m_player1(nullptr), m_player2(nullptr), m_player3(nullptr)
{
}

ZombieHandler::~ZombieHandler()
{
	// We are only clearing, because we are not responsible for players
	m_player1 = nullptr;
	m_player2 = nullptr;
	m_player3 = nullptr;
}

bool ZombieHandler::Init(Player& p1, Player& p2, Player& p3)
{
	bool success = true;
	m_player1 = &p1;
	m_player2 = &p2;
	m_player3 = &p3;

	if (m_player1 != nullptr &&
		m_player2 != nullptr &&
		m_player3 != nullptr)
	{
		success = false;
	}

	return success;
}

void ZombieHandler::HandleZombies(float dt)
{
	for (auto &zomb : m_vecZombies)
	{
		Command* command = handleInput(*zomb);
		if (command)
		{
			command->execute(*zomb, dt);
		}
	}
}

Command* ZombieHandler::handleInput(Zombie& actor)
{
	// If zombie is hit, go to dying
	if (actor.GetIsHit())
	{
		return new Die();
	}

	// If zombie can see player, chase them
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
		Vec2f::DistanceBetween(Zpos, ret->GetPosition()))
	{
		ret = m_player2;
	}

	// Check if player3 is closer then player2
	if (Vec2f::DistanceBetween(Zpos, m_player3->GetPosition()) <
		Vec2f::DistanceBetween(Zpos, ret->GetPosition()))
	{
		ret = m_player3;
	}

	return ret;
}

void ZombieHandler::AddZombie(Zombie* zombie)
{
	m_vecZombies.push_back(zombie);
}

void ZombieHandler::RemoveDestroyed()
{
	
	// Remove destroyed actors (Bullets mostly)
	// https://stackoverflow.com/questions/991335/how-to-erase-delete-pointers-to-objects-stored-in-a-vector rlbond's answer
	// Apply a deleting function to each element. (call the destructor and make the pointer null)
	std::for_each(m_vecZombies.begin(), m_vecZombies.end(), [](Zombie*& e)
		{
			if (e->GetDestroyed())
			{
				// e is not deleted here. That is done after this.
				e = nullptr;
			}
		});
	// remove re-orders the vector, so removed elements are at the back of the container
	std::vector<Zombie*>::iterator newEnd = std::remove(m_vecZombies.begin(), m_vecZombies.end(), static_cast<Zombie*>(nullptr));
	// erase the elements from the vector
	m_vecZombies.erase(newEnd, m_vecZombies.end());
	// DELETE ACTORS END
}
