#include "Player.h"
#include "ZombieSquad.h"
#include "Bullet.h"
#include "Command.h"
#include "GlobalConstants.h"
#include <iostream>


Player::Player(float x, float y, float dir, int playerNum, ZombieSquad& game, PlayerHandler& player_handler, bool startingPlayer)
	: m_game(&game), m_handler(&player_handler), m_playerNumber(playerNum)
{
	SetDestroyed(false);
	SetX(x); 
	SetY(y);
	SetRadius(PLAYER_SIZE * GAME_SCALE);
	SetDirection(dir);
	SetTag(ActorTag::PLAYER);
	m_fireRate = PLAYER_FIRING_RATE;
	m_lastTimeFired = 0.0f;

	if (startingPlayer)
	{
		m_currentState = new Controlled();
		m_currentState->Enter(*this);
	}
	else {
		m_currentState = new Watching();
		m_currentState->Enter(*this);
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

#pragma warning (disable : 4244) // converting from float to int32_t

	// player circle
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), GetColor());

	// look direction
	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection())*GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection())*GetRadius()),
		olc::BLUE);

	// players number
	game.DrawString((int32_t)GetX() + 5.0f, (int32_t)GetY() + 5.0f,
		std::to_string(m_playerNumber), olc::WHITE, GAME_SCALE);

	

#pragma warning (default : 4244)
}

void Player::Update(float dt)
{
	// Update the current Player state
	m_lastTimeFired += dt;
	m_currentState->Update(*this, dt);
}

void Player::MoveForward(float dt)
{
	SetX(GetX() + cosf(GetDirection()) * PLAYER_SPEED * dt);
	SetY(GetY() + sinf(GetDirection()) * PLAYER_SPEED * dt);
}

void Player::MoveBack(float dt)
{
	SetX(GetX() - cosf(GetDirection()) * PLAYER_SPEED * dt);
	SetY(GetY() - sinf(GetDirection()) * PLAYER_SPEED * dt);
}

void Player::TurnRight(float dt)
{
	SetDirection(GetDirection() + 2.0f* dt);
}

void Player::TurnLeft(float dt)
{
	SetDirection(GetDirection() - 2.0f* dt);
}

void Player::Attack(float)
{	// if cooldown -> fire shot
	if (m_lastTimeFired >= m_fireRate)
	{
		// calculate bullet spawn point.
		Vec2f bulletSpawnPoint = GetPosition() + Vec2f(cosf(GetDirection()), sinf(GetDirection())) * (GetRadius() + 2.0f);
		m_game->SpawnBullet(bulletSpawnPoint, GetDirection(), 5.0f, 10.0f, ActorTag::PLAYER);
		m_lastTimeFired = 0.0f;
	}
}

void Player::Die(float)
{
	if (m_currentState->GetStateID() != StateID::STATE_DEAD)
	{
		m_currentState = new PlayerDead();
		m_currentState->Enter(*this);
	}
}

void Player::ChangePlayer(bool truth)
{
	if (truth)
	{
		// This becomes the controlled player
		m_currentState = new Controlled();
		m_currentState->Enter(*this);
	}
	else {
		// this becomes an npc character
		m_currentState = new Watching();
		m_currentState->Enter(*this);
	}
}

// returns nullptr if no zombie
const Zombie* Player::GetVisibleZombie()
{
	// ask player handler for a target
	return m_handler->GetClosestVisibleZombiePosition(*this);
	
}

