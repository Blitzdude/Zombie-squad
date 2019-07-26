#pragma once
#include "Actor.h"

class ZombieSquad;
class Bullet;

class Player : public Actor
{
public:
	// Player();
	Player(float x, float y, float dir, ZombieSquad& game);
	~Player();
	virtual void Draw(olc::PixelGameEngine& game) override;

	virtual void MoveForward(float fElapsedTime) override;
	virtual void MoveBack(float fElapsedTime)	 override;
	virtual void TurnRight(float fElapsedTime)	 override;
	virtual void TurnLeft(float fElapsedTime)	 override;
	virtual void Attack(float fElapsedTime)		 override;

private:
	ZombieSquad* m_game; // Needed for bullet spawning
	// Bullet* m_gun; // Which kind of bullets to spawn when firing // Not implemented
};

