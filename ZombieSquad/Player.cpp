#include "Player.h"
#include <iostream>


Player::Player()
{
	const float PLAYER_SIZE = 10.0f;

	SetX(0.0f);
	SetY(0.0f);
	SetRadius(PLAYER_SIZE);
	SetDirection(0.0f);
}

Player::Player(float x, float y)
{
	const float PLAYER_SIZE = 10.0f;
	SetX(x); 
	SetY(y);
	SetRadius(PLAYER_SIZE);
	SetDirection(0.0f);
	std::cout << "Player created\n";

}

Player::~Player()
{
	std::cout << "Player destroyed\n";
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
