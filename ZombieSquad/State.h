#pragma once
#include "Command.h"
enum class StateID
{
	STATE_ROAM,		  // Zombie states
	STATE_CHASE,
	STATE_CONTROLLED, // Player states
	STATE_OVERWATCH,
	STATE_DEAD,		  // All Actors 
	STATE_NONE
};

class Player;

class PlayerState
{
public:
	PlayerState()
		: m_id(StateID::STATE_NONE)
	{}

	virtual void Enter(Player& player) {};
	virtual void Update(Player& player, float dt) {};
	// virtual void Exit(Player& player) {}; // Not Implemented


	virtual ~PlayerState() {};

	const StateID& getStateID() const { return m_id; };

private:
	StateID m_id;
};
/*
class Bullet;

class BulletState
{
public:
	BulletState()
		: m_id(StateID::STATE_NONE)
	{}

	virtual ~BulletState() {};

	virtual void Enter(Bullet& bullet) {};
	virtual void Update(Bullet& bullet, float dt) {};
	// virtual void Exit(Player& player) {}; // Not implemented

	const StateID& getStateID() const { return m_id; };
private:
	StateID m_id;

};
*/

class Zombie;

class ZombieState
{
public:
	ZombieState()
		: m_id(StateID::STATE_NONE)
	{}

	virtual ~ZombieState() {};

	virtual void Enter(Zombie& player) {};
	virtual void Update(Zombie& player, float dt) {};
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& getStateID() const { return m_id; };
private:
	StateID m_id;

};
