#pragma once

#include "Actor.h"

/**
 * Abstract base class for command objects
 * 
 * Commands allow for the lazy activation of actor methods. 
 * see the command pattern for more details. Each command has
 * an execute method that takes in the actor reference and delta time
 * this allows command to execute actors methods lazily
 */
class Command
{
public:
	virtual ~Command() {}

	/** 
	 * execute tells the actor to "execute" a specific command
	 *
	 * execute tells the actor to "execute" a specific command. is
	 * overriden by children
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) = 0;
};

/**
 * MoveForward command
 * 
 * @see Command
 */
class MoveForward : public Command
{
public:

	/**
	 * Default constructor
	 */
	MoveForward() {};

	/**
	 * Executes MoveForward method on actor 
	 *
	 * tells the actor to execute a move forward method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.MoveForward(dt);
	}
};

/**
 * MoveBack command
 *
 * @see Command
 */
class MoveBack : public Command
{
public:

	/**
	 * Default constructor
	 */
	MoveBack() {};

	/**
	 * Executes MoveBack method on actor
	 *
	 * tells the actor to execute a MoveBack method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.MoveBack(dt);
	}
};

/**
 * TurnRight command
 *
 * @see Command
 */
class TurnRight : public Command
{
public:

	/**
	 * Default constructor
	 */
	TurnRight() {};

	/**
	 * Executes MoveForward method on actor
	 *
	 * tells the actor to execute a move forward method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.TurnRight(dt);
	}
};

/**
 * TurnLeft command
 *
 * @see Command
 */
class TurnLeft : public Command
{
public:

	/**
	 * Default constructor
	 */
	TurnLeft() {};

	/**
	 * Executes TurnLeft method on actor
	 *
	 * tells the actor to execute a TurnLeft method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.TurnLeft(dt);
	}
};

/**
 * ChangePlayer command
 *
 * @see Command
 */
class ChangePlayer : public Command
{
public:

	/**
	 * Parameterized constructor
	 *
	 * If truth value is true, player actor goes to Controlled. 
	 * Otherwise it goes to Watching
	 * @param truth boolean value if we want to take control of this player or not
	 * 
	 */
	ChangePlayer(bool truth)
	 : m_truth(truth)
	{};

	/**
	 * Executes ChangePlayer method on actor
	 *
	 * tells the actor to execute a ChangePlayer method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.ChangePlayer(m_truth);
	}
private:
	bool m_truth;

};

/**
 * Attack command
 *
 * @see Command
 */
class Attack : public Command
{
public:

	/**
	 * Default constructor
	 */
	Attack() {};

	/**
	 * Executes Attack method on actor
	 *
	 * tells the actor to execute a Attack method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.Attack(dt);
	}
};

/**
 * Move forward command
 *
 * @see Command
 */
class ChasePlayer : public Command
{
public: 

	/**
	 * Parameterized constructor
	 * @param player const reference to a player to chase
	 */
	ChasePlayer(const Player* player) 
		: m_player(player)
	{};

	/**
	 * Executes Chase method on actor
	 *
	 * tells the actor to execute a Chase method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.Chase(*m_player);
	}
private:
	const Player* m_player;
};

/**
 * command to navigate to target location
 *
 * @see Command
 */
class NavigateToLocation : public Command
{
public:

	/** 
	 * Parameterized constructor
	 *
	 * @param target Target to navigate to.
	 */
	NavigateToLocation(Vec2f target)
		: m_target(target)
	{};

	/**
	 * Executes MoveForward method on actor
	 *
	 * tells the actor to execute a move forward method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.NavigateTo(m_target);
	}

private:
	Vec2f m_target;
};

/**
 * Die command
 *
 * @see Command
 */
class Die : public Command
{
public:

	/**
	 * Default constructor
	 */
	Die() {};

	/**
	 * Executes Die method on actor
	 *
	 * tells the actor to execute a Die method
	 * @param actor reference to an actor
	 * @param dt delta time
	 */
	virtual void execute(Actor& actor, float dt) override
	{
		actor.Die(dt);
	}
};