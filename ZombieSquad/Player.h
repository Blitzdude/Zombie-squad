#pragma once
#include "Actor.h"
#include "State.h"

class ZombieSquad;
class Bullet;
class PlayerState;

class Player : public Actor
{
public:
	Player(float x, float y, float dir, ZombieSquad& game);
	~Player();
	virtual void Draw(olc::PixelGameEngine& game) override;
	virtual void Update(float dt) override;

	virtual void MoveForward(float dt) override;
	virtual void MoveBack(float dt)	 override;
	virtual void TurnRight(float dt)	 override;
	virtual void TurnLeft(float dt)	 override;
	virtual void Attack(float dt)		 override;
	virtual void Die(float dt)	override;

	

private:
	PlayerState* m_currentState;
	ZombieSquad* m_game; // Needed for bullet spawning
	// Bullet* m_gun; // Which kind of bullets to spawn when firing // Not implemented
};

