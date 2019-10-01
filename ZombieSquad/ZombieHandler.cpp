#include "ZombieHandler.h"
#include "Command.h"
#include "StructCollection.h"
#include "GlobalConstants.h"
#include "ZombieSquad.h"
#include "Physics.h"

ZombieHandler::ZombieHandler(ZombieSquad& engine)
	: m_game(&engine)
{
}

ZombieHandler::~ZombieHandler()
{
	// We are only clearing, because we are not responsible for players
	m_vecPlayers.clear();
}

bool ZombieHandler::Init(Player& p1, Player& p2, Player& p3)
{
	bool success = true;

	m_vecPlayers.push_back(&p1);
	m_vecPlayers.push_back(&p2);
	m_vecPlayers.push_back(&p3);

	for (auto itr : m_vecPlayers)
	{
		if (itr == nullptr)
		{
			std::cout << "One or more players were nullptrs \n";
			success = false;
		}
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


	for (auto player : m_vecPlayers)
	{
		// if zombie is close enough to attack, attack
		if (player != nullptr && 
			Vec2f::DistanceBetween(actor.GetPosition(), player->GetPosition()) < ATTACK_RANGE)
		{
			return new Attack();
		}
		if (player != nullptr &&
			ZombieSeesTarget(player->GetPosition(), actor))
		{
			// If zombie can see player, chase them
			if (player->GetCurrentState()->GetStateID() != StateID::STATE_DEAD)
			{
				return new ChasePlayer(player);
			}
		}
	}
	
	// if zombie has no target, navigate to the last place
	return nullptr;
}

const Player* ZombieHandler::GetClosestPlayer(Zombie& zombie)
{
	Player* ret = nullptr;
	Vec2f Zpos = zombie.GetPosition();
	// initialize distance to max
	float distance = std::numeric_limits<float>::max();

	for (auto itr : m_vecPlayers)
	{
		if (Vec2f::DistanceBetween(itr->GetPosition(), zombie.GetPosition()) < distance)
		{
			if (itr->GetCurrentState()->GetStateID() != StateID::STATE_DEAD)
			{
				distance = Vec2f::DistanceBetween(itr->GetPosition(), zombie.GetPosition());
				ret = itr;
			}
		}
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

bool ZombieHandler::ZombieSeesTarget(const Vec2f& target, const Zombie& zombie)
{
	// is the target close enough
	if (Vec2f::DistanceBetween(target, zombie.GetPosition()) <= ZOMBIE_SIGHT_RANGE)
	{
		// get the directional vectors left and right
		Vec2f left = zombie.GetDirectionVector().GetRotated(ZOMBIE_SIGHT_FOV_RAD) + zombie.GetPosition();
		Vec2f right = zombie.GetDirectionVector().GetRotated(-ZOMBIE_SIGHT_FOV_RAD) + zombie.GetPosition();
		// if target is left-side of right and right-side of left, target is in the cone
		if (!Vec2f::IsLeft(zombie.GetPosition(), left, target) && 
			Vec2f::IsLeft(zombie.GetPosition(), right, target))
		{
			// if target can be hit with a ray, it is visible
			Ray ray(zombie.GetPosition(), target); 
			
			for (auto edge : Level::GetEdges())
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
