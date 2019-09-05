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
#pragma warning (disable : 4244) // Disable conversion from 'float' to 'int32_t' warning

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
	Vec2f left = GetDirectionVector().GetRotated(ZOMBIE_SIGHT_FOV_RAD / 2.0f);
	left = left.GetNormalized() * ZOMBIE_SIGHT_RANGE + GetPosition();

	Vec2f right = GetDirectionVector().GetRotated(-ZOMBIE_SIGHT_FOV_RAD / 2.0f);
	right = right.GetNormalized() * ZOMBIE_SIGHT_RANGE + GetPosition();
	game.DrawTriangle(GetX(), GetY(), left.x, left.y, right.x, right.y, olc::CYAN);
	
	game.DrawCircle(m_target.x, m_target.y, 1.0f, olc::MAGENTA);
	// get vector to target
	Vec2f vec = m_target - GetPosition();

	// normalize vector
	vec.Normalize();
	float angle = std::abs(Vec2f::AngleBetween(vec, GetDirectionVector()));

	game.DrawLine(GetX(), GetY(), vec.x*8.0f + GetX(), vec.y*8.0f + GetY());
	game.DrawString(GetX(), GetY(), std::to_string(angle));

#pragma warning (default : 4244) // reenable the warning

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
	float angle = std::abs(Vec2f::AngleBetween(vec, GetDirectionVector()) );

	if (angle > 0.01f)
	{
		// Which direction to turn?
		float cross = Vec2f::CrossProduct(vec, GetDirectionVector());
		float dir = cross > 0.0f ? -1.0f : 1.0f; // Magically determined

		SetDirection(GetDirection() + 0.5f * dir * dt);
	}
	
	if (Vec2f::DistanceBetween(GetPosition(), m_target) > ATTACK_RANGE 
		&& angle < 0.1f)
	{
		SetPosition(GetPosition() + vec * ZOMBIE_SPEED * dt); 
		
		//SetDirection(Vec2f::AngleBetween(Vec2f(1.0f, 0.0f), vec));
		// TODO: LEft off here, fix zombies looking away from movement direction
	}
	
}
