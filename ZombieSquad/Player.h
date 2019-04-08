#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player();
	Player(float x, float y);
	~Player();
	virtual void Draw(olc::PixelGameEngine& game) override;

	virtual void MoveForward(float fElapsedTime) override;
	virtual void MoveBack(float fElapsedTime)	 override;
	virtual void TurnRight(float fElapsedTime)	 override;
	virtual void TurnLeft(float fElapsedTime)	 override;

private:

};

