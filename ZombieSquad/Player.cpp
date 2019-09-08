#include "Player.h"
#include "ZombieSquad.h"
#include "Bullet.h"
#include "Command.h"
#include "GlobalConstants.h"
#include <iostream>


Player::Player(float x, float y, float dir, ZombieSquad& game, PlayerHandler& player_handler, bool startingPlayer)
	: m_game(&game), m_handler(&player_handler)
{
	SetDestroyed(false);
	SetX(x); 
	SetY(y);
	SetRadius(PLAYER_SIZE);
	SetDirection(dir);
	SetTag(ActorTag::PLAYER);
	if (startingPlayer)
	{
		m_currentState = new Controlled();
	}
	else {
		m_currentState = new Watching();
	}
	std::cout << "Player created\n";
}

Player::~Player()
{
	std::cout << "Player destroyed\n";
	// Free pointers
	m_game = nullptr;
	delete m_currentState;
	m_currentState = nullptr;

}

void Player::Draw(olc::PixelGameEngine& game)
{
	olc::Pixel pix;
	pix = m_currentState->GetStateID() != StateID::PLAYER_CONTROLLED ? olc::WHITE : olc::CYAN;

	if (m_currentState->GetStateID() == StateID::STATE_DEAD)
	{
		pix = olc::VERY_DARK_RED;
	}

	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), pix);

	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection())*GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection())*GetRadius()),
		olc::BLUE);

	game.DrawString((int32_t)GetX() + 5.0f, (int32_t)GetY() + 5.0f, std::to_string(GetDirection()));
}

void Player::Update(float dt)
{
	// Update the current Player state
	m_currentState->Update(*this, dt);
}

void Player::MoveForward(float dt)
{
	SetX(GetX() + cosf(GetDirection())*100.0f* dt);
	SetY(GetY() + sinf(GetDirection())*100.0f* dt);
}

void Player::MoveBack(float dt)
{
	SetX(GetX() - cosf(GetDirection())*100.0f* dt);
	SetY(GetY() - sinf(GetDirection())*100.0f* dt);
}

void Player::TurnRight(float dt)
{
	SetDirection(GetDirection() + 2.0f* dt);
}

void Player::TurnLeft(float dt)
{
	SetDirection(GetDirection() - 2.0f* dt);
}

void Player::Attack(float dt)
{	// if cooldown and bullets -> fire shot
	// calculate bullet spawn point.
	Vec2f bulletSpawnPoint = GetPosition() + Vec2f(cosf(GetDirection()), sinf(GetDirection())) * (GetRadius() + 2.0f);
	m_game->SpawnBullet(bulletSpawnPoint, GetDirection(), 5.0f, 10.0f, ActorTag::PLAYER);
}

void Player::Die(float dt)
{
	if (m_currentState->GetStateID() != StateID::STATE_DEAD)
	{
		m_currentState = new PlayerDead();
	}
}

void Player::ChangePlayer(bool truth)
{
	if (truth)
	{
		// This becomes the controlled player
		m_currentState = new Controlled();
	}
	else {
		// this becomes an npc character
		m_currentState = new Watching();
	}
}

// returns nullptr
const Zombie* Player::GetVisibleZombie()
{
	// ask player handler for a target
	return m_handler->GetClosestVisibleZombiePosition(*this);
	
}

