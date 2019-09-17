#include "State.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"
#include "GlobalConstants.h"

void Chasing::Update(Zombie& zombie, float dt)
{	
	zombie.SetTarget(m_chaseTarget->GetPosition());
	zombie.doMove(dt);
}

void Controlled::Update(Player& player, float dt)
{
	// Take input commands
}

void Watching::Update(Player& player, float dt)
{
	// Get closest zombie
	const Zombie* targetZombie = player.GetVisibleZombie();
	if (targetZombie != nullptr)
	{
		// If zombie is in front of player, turn to face the zombie
		float angle = std::abs(Vec2f::AngleBetween(player.GetDirectionVector(), targetZombie->GetPosition() - player.GetPosition() ));

		if (angle > 0.05f) // MAGIC
		{
			// Which direction to turn?
			float cross = Vec2f::CrossProduct(targetZombie->GetPosition() - player.GetPosition(), player.GetDirectionVector());
			// turn accordingly
			cross < 0.0f ? player.TurnRight(dt) : player.TurnLeft(dt);
		}
		else 
		{
			// Zombie in front. Fire the gun!
			player.Attack(dt);
		}
	}
	else {
		// otherwise turn to face the direction you are watching
		// TODO: have the watching player actor turn back to the direction they are looking at
	}
}


void Flying::Update(Bullet& bullet, float dt)
{
	float dir = bullet.GetDirection();
	// New position
	Vec2f newPos(bullet.GetPosition() + Vec2f(cosf(dir), sinf(dir)) * BULLET_SPEED * dt);
	bullet.SetPosition(newPos);

	bullet.m_lifeTime -= dt;
	if (bullet.GetIsHit() || bullet.m_lifeTime < 0.0f)
	{
		bullet.SetDestroyed(true);
	}
}

void Roaming::Update(Zombie& zombie, float dt)
{
	// add to timer
	m_timer += dt;
	if (m_timer >= TIME_UNTIL_CHANGE_DIR)
	{
		m_timer = 0.0f;
		// randomize direction
		Vec2f r = zombie.GetRandomCellLocation();
		zombie.SetTarget(r);
	}

	zombie.doMove(dt);
}

void ZombieDead::Update(Zombie& zombie, float dt)
{
	m_deathTime += dt;
	if (m_deathTime > DYING_TIME)
	{
		zombie.SetDestroyed(true);
	}
}

void PlayerDead::Update(Player&, float dt)
{
	m_deathTime += dt;
	// Do not destroy players! just lie there... dead
}

void Navigating::Update(Zombie& zombie, float dt)
{
	// get path from level
		// go to the next cell in the list
		// if the zombie is on the current cell in the list -> pop_back
		// repeat until list is gone. 
	// then get the list again. 
}
