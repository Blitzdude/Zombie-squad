#include "Bullet.h"
#include "State.h"
#include "GlobalConstants.h"
#include "ZombieSquad.h"

Bullet::Bullet(const Vec2f& pos, float dir, float speed, float lifeTime, ActorTag owner)
	: m_lifeTime(lifeTime), m_radius(0.0f), m_owner(owner)
{
	std::cout << "Bullet created\n";
	m_currentState = new Flying();
	SetDestroyed(false);
	SetPosition(pos);
	SetDirection(dir);
	SetSpeed(speed);
	SetRadius(BULLET_SIZE * GAME_SCALE);
	SetColor(olc::YELLOW);
	SetTag(ActorTag::BULLET);
}

Bullet::~Bullet()
{
	delete m_currentState;
	m_currentState = nullptr;
}

void Bullet::Draw(ZombieSquad& game)
{
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), GetColor());
}

void Bullet::Update(float fElapsedTime)
{
	m_currentState->Update(*this, fElapsedTime);
}

