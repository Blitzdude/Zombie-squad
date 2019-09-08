#include "Bullet.h"
#include "State.h"
#include "GlobalConstants.h"

Bullet::Bullet(const Vec2f& pos, float dir, float s, float l, ActorTag owner)
	: m_lifeTime(l), m_radius(0.0f), m_owner(owner)
{
	std::cout << "Bullet created\n";
	m_currentState = new Flying();
	SetDestroyed(false);
	SetPosition(pos);
	SetDirection(dir);
	SetSpeed(s);
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
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius());
}

void Bullet::Update(float fElapsedTime)
{
	m_currentState->Update(*this, fElapsedTime);
}

