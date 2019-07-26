#include "Player.h"
#include "ZombieSquad.h"
#include "Bullet.h"
#include <iostream>

/* DELETED
Player::Player()
{
	const float PLAYER_SIZE = 6.0f;

	SetX(0.0f);
	SetY(0.0f);
	SetRadius(PLAYER_SIZE);
	SetDirection(0.0f);
}
*/

Player::Player(float x, float y, float dir, ZombieSquad& game)
	: m_game(&game)
{
	const float PLAYER_SIZE = 6.0f;
	SetX(x); 
	SetY(y);
	SetRadius(PLAYER_SIZE);
	SetDirection(dir);
	std::cout << "Player created\n";

}

Player::~Player()
{
	std::cout << "Player destroyed\n";
	// Free pointers
	m_game = nullptr;
	

}


void Player::Draw(olc::PixelGameEngine& game)
{

	// TODO: Size should be a variable, static maybe? 
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius());

	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection())*GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection())*GetRadius()),
		olc::BLUE);

}

void Player::MoveForward(float fElapsedTime)
{
	SetX(GetX() + cosf(GetDirection())*100.0f*fElapsedTime);
	SetY(GetY() + sinf(GetDirection())*100.0f*fElapsedTime);
}

void Player::MoveBack(float fElapsedTime)
{
	SetX(GetX() - cosf(GetDirection())*100.0f*fElapsedTime);
	SetY(GetY() - sinf(GetDirection())*100.0f*fElapsedTime);
}

void Player::TurnRight(float fElapsedTime)
{
	SetDirection(GetDirection() + 2.0f*fElapsedTime);
}

void Player::TurnLeft(float fElapsedTime)
{
	SetDirection(GetDirection() - 2.0f*fElapsedTime);
}

void Player::Attack(float fElapsedTime)
{	// if cooldown and bullets -> fire shot
	// calculate bullet spawn point.
	Vec2f bulletSpawnPoint = GetPosition() + Vec2f(cosf(GetDirection()), sinf(GetDirection())) * (GetRadius() + 2.0f);
	m_game->SpawnBullet(bulletSpawnPoint, GetDirection(), 5.0f);
}
