#include "Bullet.h"


Bullet::Bullet(const Vec2f& pos, float dir, float l)
	: m_lifeTime(l), m_radius(0.0f)
{
	const float BULLET_SIZE = 4.0f;
	SetPosition(pos);
	SetDirection(dir);
	SetRadius(BULLET_SIZE);
	SetTag(ActorTag::BULLET);
}
void Bullet::Draw(olc::PixelGameEngine& game)
{
	game.FillCircle(GetX(), GetY(), GetRadius());
}
