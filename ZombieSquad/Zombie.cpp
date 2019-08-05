#include "Zombie.h"
#include "State.h"

Zombie::Zombie(float x, float y)
{
	
	m_currentState = new Roaming();
	const float ZOMBIE_SIZE = 6.0f;
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
	olc::Pixel pix = m_currentState->GetStateID() == StateID::ZOMBIE_CHASE ? olc::DARK_YELLOW : olc::DARK_GREEN;
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), pix);

	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection()) * GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection()) * GetRadius()),
		olc::RED);

}

void Zombie::Update(float fElapsedTime)
{
	m_currentState->Update(*this, fElapsedTime);
}

void Zombie::Chase(const Player& player)
{
	if (m_currentState->GetStateID() != StateID::ZOMBIE_CHASE)
	{
		m_currentState = new Chasing(player);
	}
}

void Zombie::doMove(float dt)
{
	// get vector to target
	Vec2f vec = m_target - GetPosition();
	
	// normalize vector
	vec.Normalize();

	if (Vec2f::DistanceBetween(GetPosition(), m_target) > 1.0f)
	{
		SetPosition(GetPosition() + vec * 50.0f * dt); // Zombie speed = 50
		SetDirection(Vec2f::AngleBetween(Vec2f(1.0f, 0.0f), vec));
	}
}
