#include "Bullet.h"
#include "State.h"

Bullet::Bullet(const Vec2f& pos, float dir, float l)
	: m_lifeTime(l), m_radius(0.0f)
{
	m_currentState = new Flying();
	const float BULLET_SIZE = 4.0f;
	SetDestroyed(false);
	SetPosition(pos);
	SetDirection(dir);
	SetRadius(BULLET_SIZE);
	SetTag(ActorTag::BULLET);
}

Bullet::~Bullet()
{
	delete m_currentState;
	m_currentState = nullptr;
}

void Bullet::Draw(olc::PixelGameEngine& game)
{
	game.FillCircle(GetX(), GetY(), GetRadius());
}

void Bullet::Update(float fElapsedTime)
{
	m_currentState->Update(*this, fElapsedTime);
	// TODO: Update the bullet, maybe bullets shoudl have states as everyone else... or maybe not idk. yes? no?
	

}
