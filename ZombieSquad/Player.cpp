#include "Player.h"
#include "ZombieSquad.h"
#include "Bullet.h"
#include "Command.h"
#include "GlobalConstants.h"
#include <iostream>


Player::Player(float x, float y, float dir, ZombieSquad& game)
	: m_game(&game)
{
	SetDestroyed(false);
	SetX(x); 
	SetY(y);
	SetRadius(PLAYER_SIZE);
	SetDirection(dir);
	SetTag(ActorTag::PLAYER);
	m_currentState = new Watching();
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
	m_game->SpawnBullet(bulletSpawnPoint, GetDirection(), 5.0f, 40.0f, ActorTag::PLAYER);
}

void Player::Die(float dt)
{
	if (m_currentState->GetStateID() != StateID::STATE_DEAD)
	{
		delete m_currentState;
		m_currentState = new PlayerDead();
	}
}

