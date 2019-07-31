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
