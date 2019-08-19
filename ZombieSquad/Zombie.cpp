#include "ZombieSquad.h"
#include "Zombie.h"
#include "State.h"
#include "GlobalConstants.h"


Zombie::Zombie(float x, float y, ZombieSquad& game)
	: m_game(&game)
{
	m_currentState = new Roaming();
	SetDestroyed(false);
	SetX(x);
	SetY(y);
	m_target = GetPosition();
	SetRadius(ZOMBIE_SIZE);
	SetDirection(0.0f);
	SetTag(ActorTag::ZOMBIE);
	std::cout << "Zombie created\n";
}

Zombie::~Zombie()
{
	std::cout << "Zombie destroyed\n";
}


void Zombie::Draw(olc::PixelGameEngine& game)
{

	// TODO: Size should be a variable, static maybe? 
	olc::Pixel pix = olc::DARK_GREEN;
	if (m_currentState->GetStateID() == StateID::ZOMBIE_CHASE)
	{
		pix = olc::DARK_YELLOW;
	}
	else if (m_currentState->GetStateID() == StateID::STATE_DEAD)
	{
		pix = olc::DARK_RED;
	}

	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), pix);

	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection()) * GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection()) * GetRadius()),
		olc::RED);

}

void Zombie::Update(float dt)
{
	m_currentState->Update(*this, dt);
}

void Zombie::Chase(const Player& player)
{
	if (m_currentState->GetStateID() != StateID::ZOMBIE_CHASE)
	{
		m_currentState = new Chasing(player);
	}
}

void Zombie::Die(float dt)
{
	if (m_currentState->GetStateID() != StateID::STATE_DEAD)
	{
		delete m_currentState;
		m_currentState = new ZombieDead();
	}
}

void Zombie::Attack(float dt)
{
	// calculate position for bullet 
	Vec2f bulletSpawnPoint = GetPosition() + Vec2f(cosf(GetDirection()), sinf(GetDirection())) * (GetRadius() + 2.0f);
	// Spawn the bullet
	m_game->SpawnBullet(bulletSpawnPoint, GetDirection(), 1.0f, 0.0f, ActorTag::ZOMBIE);
}

void Zombie::doMove(float dt)
{
	// get vector to target
	Vec2f vec = m_target - GetPosition();
	
	// normalize vector
	vec.Normalize();

	if (Vec2f::DistanceBetween(GetPosition(), m_target) > ATTACK_RANGE)
	{
		SetPosition(GetPosition() + vec * ZOMBIE_SPEED * dt); // Zombie speed = 50
		SetDirection(Vec2f::AngleBetween(Vec2f(1.0f, 0.0f), vec));
	}
}
