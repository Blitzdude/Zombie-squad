#include "Player.h"
#include <iostream>
Player::Player()
{
	SetX(0.0f);
	SetY(0.0f);
	SetDirection(0.0f);
}

Player::Player(float x, float y)
{
	SetX(x); 
	SetY(y);
	SetDirection(0.0f);
	std::cout << "Player created\n";

}

Player::~Player()
{
	std::cout << "Player destroyed\n";
}


void Player::Draw(olc::PixelGameEngine& game)
{
	const float PLAYER_SIZE = 10.0f;

	// TODO: Size should be a variable, static maybe? 
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)PLAYER_SIZE);

	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection())*PLAYER_SIZE),
		(int32_t)(GetY() + sinf(GetDirection())*PLAYER_SIZE),
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
	SetDirection(GetDirection() + 10.0f*fElapsedTime);
}

void Player::TurnLeft(float fElapsedTime)
{
	SetDirection(GetDirection() - 10.0f*fElapsedTime);
}
