#pragma once
#include "Command.h"
enum class StateID
{
	ZOMBIE_ROAM,	   // Zombie states
	ZOMBIE_CHASE,
	PLAYER_CONTROLLED, // Player states
	PLAYER_OVERWATCH,
	STATE_DEAD,		   // All Actors 
	STATE_NONE
};

class Player;

class PlayerState
{
public:
	// Abstract class to give a common update Functionality for Actors
	PlayerState() {}
	virtual ~PlayerState() {};

	virtual void Update(Player& actor, float dt) = 0;
};

class Zombie;

class ZombieState
{
public:
	// Abstract class to give a common update Functionality for Actors
	ZombieState() {}
	virtual ~ZombieState() {};

	virtual void Update(Zombie& actor, float dt) = 0;
};

class Bullet;

class BulletState
{
public:
	// Abstract class to give a common update Functionality for Actors
	BulletState() {}
	virtual ~BulletState() {};

	virtual void Update(Bullet& actor, float dt) = 0;
};

class Controlled : public PlayerState
{
public:
	Controlled()
		: m_id(StateID::PLAYER_CONTROLLED)
	{}
	virtual ~Controlled() {};

	virtual void Enter(Player& actor) {};
	virtual void Update(Player& actor, float dt);
	// virtual void Exit(Player& player) {}; // Not Implemented

	const StateID& getStateID() const { return m_id; };

private:
	StateID m_id;
};

class Overwatch : public PlayerState
{
public:
	Overwatch()
		: m_id(StateID::PLAYER_OVERWATCH)
	{}
	virtual ~Overwatch() {};

	virtual void Enter(Player& actor) {};
	virtual void Update(Player& actor, float dt);
	// virtual void Exit(Player& player) {}; // Not Implemented

	const StateID& getStateID() const { return m_id; };

private:
	StateID m_id;
};

class Chase : public ZombieState
{
public:
	Chase()
		: m_id(StateID::ZOMBIE_CHASE)
	{}

	virtual ~Chase() {};

	virtual void Enter(Zombie& actor) {};	// Not implemented
	virtual void Update(Zombie& actor, float dt);
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& getStateID() const { return m_id; };
private:
	StateID m_id;

};

class Flying : public BulletState
{
public:
	Flying()
		: m_id(StateID::STATE_NONE)
	{}

	virtual ~Flying() {};

	virtual void Enter(Bullet& actor) {};
	virtual void Update(Bullet& actor, float dt);
	// virtual void Exit(Player& player) {}; // Not implemented

	const StateID& getStateID() const { return m_id; };
private:
	StateID m_id;

};

