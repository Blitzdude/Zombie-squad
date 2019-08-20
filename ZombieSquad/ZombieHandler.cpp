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
	// if zombie is close enough to attack, attack
	if (player != nullptr && 
		Vec2f::DistanceBetween(actor.GetPosition(), player->GetPosition()) < 20.0f)
	{
		return new Attack();
	}
	else if (player != nullptr &&
		Vec2f::DistanceBetween(actor.GetPosition(), player->GetPosition()) <= SIGHT_RANGE)
	{
		return new ChasePlayer(player);
	}
	
	return nullptr;
}

const Player* ZombieHandler::GetClosestPlayer(Zombie& zombie)
{
	Player* ret = nullptr;
	Vec2f Zpos = zombie.GetPosition();


	if (m_player1->GetCurrentState()->GetStateID() != StateID::STATE_DEAD)
	{
		// Check player 1 isn't dead
		ret = m_player1;
	}
	if (m_player2->GetCurrentState()->GetStateID() != StateID::STATE_DEAD &&
		Vec2f::DistanceBetween(Zpos, m_player2->GetPosition()) <
		Vec2f::DistanceBetween(Zpos, m_player1->GetPosition())
		)
	{
		// Check if player2 is closer then player1 and not dead
		ret = m_player2;
	}
	if (m_player3->GetCurrentState()->GetStateID() != StateID::STATE_DEAD &&
		Vec2f::DistanceBetween(Zpos, m_player3->GetPosition()) <
		Vec2f::DistanceBetween(Zpos, m_player2->GetPosition()) )
	{
		// Check if player3 is closer then player2
		ret = m_player3;
	}
	// If all of these fail, All players are dead -> return nullptr
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