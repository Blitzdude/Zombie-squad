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
	virtual void Update(float dt)  override;
	virtual void Chase(const Player& player) override;
	virtual void Die(float dt) override;
	void doMove(float dt);

	// Getters

	// Setters
	void SetTarget(const Vec2f& newTarget) { m_target = newTarget; }
private:
	ZombieState* m_currentState; 
	Vec2f m_target;
};

