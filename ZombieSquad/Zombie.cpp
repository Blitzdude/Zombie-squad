#include "ZombieSquad.h"
#include "Zombie.h"
#include "State.h"
#include "GlobalConstants.h"


Zombie::Zombie(float x, float y, ZombieSquad& game, ZombieHandler& handler)
	: m_game(&game), m_handler(&handler)
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
	
	// Draw zombie
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), pix);

	// Draw look direction
	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection()) * GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection()) * GetRadius()),
		olc::RED);

	// Draw Sight Range
	Vec2f left = GetDirectionVector().GetRotated(ZOMBIE_SIGHT_FOV_RAD);
	left = left.GetNormalized() * ZOMBIE_SIGHT_RANGE + GetPosition();

	Vec2f right = GetDirectionVector().GetRotated(-ZOMBIE_SIGHT_FOV_RAD);
	right = right.GetNormalized() * ZOMBIE_SIGHT_RANGE + GetPosition();

	game.DrawTriangle(GetX(), GetY(), left.x, left.y, right.x, right.y, olc::CYAN);
	// game.DrawCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)ZOMBIE_SIGHT_RANGE, olc::CYAN);
	// game.DrawLine((int32_t)GetX(), (int32_t)GetY(), left.x, left.y, olc::CYAN);
	// game.DrawLine((int32_t)GetX(), (int32_t)GetY(), right.x, right.y, olc::CYAN);

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
	/*
	if (Vec2f::AngleBetween(vec, GetDirectionVector()) < 0.1f)
	{
		SetDirection(GetDirection() + 1.0f * dt);
	}
	*/
	if (Vec2f::DistanceBetween(GetPosition(), m_target) > ATTACK_RANGE)
	{
		SetPosition(GetPosition() + vec * ZOMBIE_SPEED * dt); // Zombie speed = 50
		SetDirection(Vec2f::AngleBetween(Vec2f(1.0f, 0.0f), vec));
	}
}
