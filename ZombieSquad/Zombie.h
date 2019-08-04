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

	void doMove(float dt);

	// Setters
	void SetTarget(const Vec2f& newTarget) { m_target = newTarget; }
private:
	ZombieState* m_currentState; // Not Implemented
	Vec2f m_target;
};

