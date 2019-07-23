#pragma once
#include "Actor.h"
class Bullet :
	public Actor
{
public:
	Bullet(const Vec2f& pos, float dir, float l) 
	 : m_lifeTime(l), m_radius(0.0f)
	{
		const float BULLET_SIZE = 4.0f;
		SetPosition(pos);
		SetDirection(dir);
		SetRadius(BULLET_SIZE);
	}

	void Draw(olc::PixelGameEngine& game) override;
	

private:
	float m_lifeTime;
	float m_radius;

};

