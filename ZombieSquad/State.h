#pragma once
#include "Command.h"
#include "GlobalConstants.h"
enum class StateID
{
	ZOMBIE_ROAM,	   // Zombie states
	ZOMBIE_CHASE,
	ZOMBIE_NAVIGATING,
	ZOMBIE_ATTACKING,
	PLAYER_CONTROLLED, // Player states
	PLAYER_OVERWATCH,
	STATE_DEAD,		   // All Actors 
	STATE_NONE
};

class Player;
class Zombie;
class Bullet;

class PlayerState
{
public:
	// Abstract class to give a common update Functionality for Actors
	PlayerState() {}
	virtual ~PlayerState() {}

	virtual void Enter(Player& player) = 0;
	virtual void Update(Player& player, float dt) = 0;

	virtual const StateID& GetStateID() = 0;
};


class ZombieState
{
public:
	// Abstract class to give a common update Functionality for Actors
	ZombieState() {}
	virtual ~ZombieState() {};

	virtual void Update(Zombie& zombie, float dt) = 0;
	virtual void Enter(Zombie& zombie) = 0;

	virtual const StateID& GetStateID() = 0;
};


class BulletState
{
public:
	// Abstract class to give a common update Functionality for Actors
	BulletState() {}
	virtual ~BulletState() {};

	virtual void Update(Bullet& bullet, float dt) = 0;

	virtual const StateID& GetStateID() = 0;
};

// PLAYER STATES
/////////////////
class Controlled : public PlayerState
{
public:
	Controlled()
		: m_id(StateID::PLAYER_CONTROLLED)
	{}
	virtual ~Controlled() {};

	virtual void Enter(Player& player) override;
	virtual void Update(Player&, float dt) override;
	// virtual void Exit(Player& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
};

class Watching : public PlayerState
{
public:
	Watching()
		: m_id(StateID::PLAYER_OVERWATCH)
	{}
	virtual ~Watching() {};

	virtual void Enter(Player& player) override;
	virtual void Update(Player& player, float dt) override;
	// virtual void Exit(Player& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	Vec2f m_direction;
	StateID m_id;
};

class PlayerDead : public PlayerState
{
public:
	PlayerDead()
		: m_id(StateID::STATE_DEAD)
	{
		m_deathTime = 0.0f;
	}

	virtual void Enter(Player& player) override; 
	virtual void Update(Player& player, float dt) override;
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	float m_deathTime;
};

// ZOMBIE STATES
/////////////////
class Chasing : public ZombieState
{
public:
	Chasing(const Player& player)
		: m_id(StateID::ZOMBIE_CHASE), m_chaseTarget(&player)
	{}

	virtual ~Chasing() { m_chaseTarget = nullptr; };

	virtual void Enter(Zombie& zombie) override;
	virtual void Update(Zombie& zombie, float dt) override;
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	const Player* m_chaseTarget;
};

class Roaming : public ZombieState
{
public:
	Roaming()
		: m_id(StateID::ZOMBIE_ROAM), m_timer(0.0f), m_timeToChangeDir(0.0f)
	{
		m_timeToChangeDir = (((std::rand() % 101) / 100.0f) * TIME_UNTIL_CHANGE_DIR) + MIN_TIME_UNTIL_CHANGE_DIR;
	}

	virtual ~Roaming() {};

	virtual void Enter(Zombie& zombie) override;
	virtual void Update(Zombie& zombie, float dt) override;
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	float m_timer;
	float m_timeToChangeDir;

};

class Navigating : public ZombieState
{
public:
	Navigating(const Vec2f& target)
		: m_id(StateID::ZOMBIE_NAVIGATING), m_timer(0.0f), m_target(target)	{}

	virtual ~Navigating() {};

	virtual void Enter(Zombie& zombie) override;
	virtual void Update(Zombie& zombie, float dt) override;

	const StateID& GetStateID() override { return m_id; }
private:
	Vec2f m_target;
	std::vector<std::pair<int, int>> m_path;
	StateID m_id;
	float m_timer;
};

class ZombieDead : public ZombieState
{
public:
	ZombieDead()
		: m_id(StateID::STATE_DEAD) 
	{
		m_deathTime = 0.0f;
	}

	virtual void Enter(Zombie& zombie) override; 	
	virtual void Update(Zombie& zombie, float dt) override;
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	float m_deathTime;
};

class ZombieAttacking : public ZombieState
{
public:
	ZombieAttacking()
		: m_id(StateID::ZOMBIE_ATTACKING), m_timer(0.0f), m_hasAttacked(false)
	{}

	virtual void Enter(Zombie& zombie) override;
	virtual void Update(Zombie& zombie, float dt) override;
	// virtual void Exit(Zombie& player) {}; // Not Implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	float m_timer;
	bool m_hasAttacked;
};


class Flying : public BulletState
{
public:
	Flying()
		: m_id(StateID::STATE_NONE)
	{}

	virtual ~Flying() {};

	virtual void Enter(Bullet& bullet) {};
	virtual void Update(Bullet& bullet, float dt) override;
	// virtual void Exit(Player& player) {}; // Not implemented

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;

};

