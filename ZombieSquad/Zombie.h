#pragma once
#include "Actor.h"
#include "State.h"

class Zombie : public Actor
{
public:
	// Zombie();
	Zombie(float x, float y);
	~Zombie();

	virtual void Draw(olc::PixelGameEngine& game) override;
	virtual void Update(float fElapsedTime) override;
private:
	ZombieState* m_currentState; // Not Implemented
};

