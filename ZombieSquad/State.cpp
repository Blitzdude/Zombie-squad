#include "State.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"

void Chase::Update(Zombie& actor, float dt)
{
}

void Controlled::Update(Player& actor, float dt)
{
}

void Overwatch::Update(Player& actor, float dt)
{
}


void Flying::Update(Bullet& actor, float dt)
{
	 
	float dir = actor.GetDirection();
	// New position
	const float BULLET_SPEED = 80.0f;
	Vec2f newPos(actor.GetPosition() + Vec2f(cosf(dir), sinf(dir)) * BULLET_SPEED * dt);
	actor.SetPosition(newPos);

	actor.m_lifeTime -= dt;
	if (actor.m_lifeTime < 0.0f)
	{
		actor.SetDestroyed(true);
	}
}

void Roam::Update(Zombie& actor, float dt)
{
	// add to timer
	m_timer += dt;
	static const float TIME_UNTIL_CHANGE_DIR = 3.0f;
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
