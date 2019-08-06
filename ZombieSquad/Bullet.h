#pragma once
#include "Actor.h"
#include "State.h"

class Bullet :
	public Actor
{
public:
	Bullet(const Vec2f& pos, float dir, float l);
	~Bullet();
	void Draw(olc::PixelGameEngine& game) override;
	void Update(float fElapsedTime) override;

	float m_lifeTime;
private:

	BulletState* m_currentState;
	float m_radius;
};

