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

/*
 * PlayerState base class
 *
 * PlayerState is an abstract base class for all player state objects
 * Each state has an Enter and Update which allow different behaviours
 */
class PlayerState
{
public:
	
	PlayerState() {}
	virtual ~PlayerState() {}

	virtual void Enter(Player& player) = 0;
	virtual void Update(Player& player, float dt) = 0;

	virtual const StateID& GetStateID() = 0;
};

/*
 * ZombieState base class
 *
 * ZombieState is an abstract base class for all zombie state objects
 * Each state has an Enter and Update which allow different behaviours
 */
class ZombieState
{
public:
	ZombieState() {}
	virtual ~ZombieState() {};

	virtual void Update(Zombie& zombie, float dt) = 0;
	virtual void Enter(Zombie& zombie) = 0;

	virtual const StateID& GetStateID() = 0;
};

/*
 * BulletState base class
 *
 * BulletState is an abstract base class for all bullet state objects
 * Each state has an Enter and Update which allow different behaviours
 */
class BulletState
{
public:
	BulletState() {}
	virtual ~BulletState() {};

	virtual void Update(Bullet& bullet, float dt) = 0;

	virtual const StateID& GetStateID() = 0;
};

// PLAYER STATES
/////////////////

/**
* Controlled state
*
* In this state the player is taking inputs from user keyboard
*/
class Controlled : public PlayerState
{
public:
	Controlled()
		: m_id(StateID::PLAYER_CONTROLLED)
	{}
	virtual ~Controlled() {};

	virtual void Enter(Player& player) override;
	virtual void Update(Player&, float dt) override;

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
};

/**
* Watching state
*
* In this state the player will automatically fire at visible zombies
*/
class Watching : public PlayerState
{
public:
	Watching()
		: m_id(StateID::PLAYER_OVERWATCH)
	{}
	virtual ~Watching() {};

	virtual void Enter(Player& player) override;
	virtual void Update(Player& player, float dt) override;

	const StateID& GetStateID() override { return m_id; }
private:
	Vec2f m_direction;
	StateID m_id;
};

/**
* PlayerDead state
*
* In this state the player cannot be controlled and is considered dead
*/
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

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	float m_deathTime;
};

// ZOMBIE STATES
/////////////////

/**
* Chasing state
*
* In this state the zombie chases the target player
*/
class Chasing : public ZombieState
{
public:
	Chasing(const Player& player)
		: m_id(StateID::ZOMBIE_CHASE), m_chaseTarget(&player)
	{}

	virtual ~Chasing() { m_chaseTarget = nullptr; };

	virtual void Enter(Zombie& zombie) override;
	virtual void Update(Zombie& zombie, float dt) override;

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;
	const Player* m_chaseTarget;
};

/**
* Chasing state
*
* In this state the zombie picks a random location in a random neighboring cell and moves there
*/
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

	const StateID& GetStateID() override { return m_id; }
private:
	StateID m_id;

};

