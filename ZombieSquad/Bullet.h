#pragma once
#include "Actor.h"
class Bullet :
	public Actor
{
public:
	Bullet(const Vec2f& pos, float dir, float l) 
	 : m_lifeTime(l)
	{
		SetPosition(pos);
		SetDirection(dir);
	}

	void Draw(olc::PixelGameEngine& game) override;
	

private:
	float m_lifeTime;
	float m_radius;

};

