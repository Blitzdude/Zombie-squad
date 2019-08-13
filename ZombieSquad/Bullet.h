#pragma once
#include "Actor.h"
#include "State.h"

class Bullet :
	public Actor
{
public:
	Bullet(const Vec2f& pos, float dir, float s, float l, ActorTag owner);
	~Bullet();
	void Draw(olc::PixelGameEngine& game) override;
	void Update(float fElapsedTime) override;

	const ActorTag& GetOwner() { return m_owner; };

	float m_lifeTime;
private:

	ActorTag m_owner;
	BulletState* m_currentState;
	float m_radius;
};

