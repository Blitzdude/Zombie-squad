#include "State.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"
#include "GlobalConstants.h"

void Chasing::Update(Zombie& actor, float dt)
{	
	actor.SetTarget(m_chaseTarget->GetPosition());
	actor.doMove(dt);
}

void Controlled::Update(Player& actor, float dt)
{
	// Take input commands
}

void Watching::Update(Player& actor, float dt)
{
	// Get closest zombie

	
	// If zombie is in front of player, fire the weapon
	// otherwise turn
}


void Flying::Update(Bullet& actor, float dt)
{
	 
	float dir = actor.GetDirection();
	// New position
	Vec2f newPos(actor.GetPosition() + Vec2f(cosf(dir), sinf(dir)) * BULLET_SPEED * dt);
	actor.SetPosition(newPos);

	actor.m_lifeTime -= dt;
	if (actor.GetIsHit() || actor.m_lifeTime < 0.0f)
	{
		actor.SetDestroyed(true);
	}
}

void Roaming::Update(Zombie& actor, float dt)
{
	// add to timer
	m_timer += dt;
	if (m_timer >= TIME_UNTIL_CHANGE_DIR)
	{
		m_timer = 0.0f;
		// randomize direction
		float rX = ((rand() % (2 * 100)) - 100) / 100.0f; // returns value between -1.0f / 1.0f 
		float rY = ((rand() % (2 * 100)) - 100) / 100.0f; // returns value between -1.0f / 1.0f
		Vec2f r(rX, rY);
		
		actor.SetTarget(actor.GetPosition() + r * 100.0f);
	}

	actor.doMove(dt);
}

void ZombieDead::Update(Zombie& actor, float dt)
{
	m_deathTime += dt;
	if (m_deathTime > DYING_TIME)
	{
		actor.SetDestroyed(true);
	}
}

void PlayerDead::Update(Player& actor, float dt)
{
	m_deathTime += dt;
	// Do not destroy players! just lay there
	
}
