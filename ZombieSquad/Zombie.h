#pragma once
// #include "ZombieSquad.h"
#include "Actor.h"
#include "State.h"

class ZombieSquad;
class ZombieHandler;

class Zombie : public Actor
{
public:
	// Zombie();
	Zombie(float x, float y, ZombieSquad& game, ZombieHandler& handler);
	~Zombie();

	virtual void Draw(olc::PixelGameEngine& game) override;
	virtual void Update(float dt)  override;
	virtual void Chase(const Player& player) override;
	virtual void Die(float dt) override;
	virtual void Attack(float dt) override;
	virtual void NavigateTo(const Vec2f& target) override;
	
	bool SeesTarget();

	void SpawnBullet(float dt);
	void doMove(float dt);
	void doRoam();
	void doNavigateTo();
	Vec2f GetRandomCellLocation();

	// Setters
	// void SetHasTarget(bool truth) { m_hasTarget = truth; }
	void SetTarget(const Vec2f& newTarget) { m_target = newTarget; }
	// bool GetHasTarget() { return m_hasTarget; }
private:
	//bool m_hasTarget = false;

	ZombieState* m_currentState; 
	ZombieHandler* m_handler;
	ZombieSquad* m_game; // Needed for bullet spawning
	Vec2f m_target;
};