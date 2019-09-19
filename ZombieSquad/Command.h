#pragma once

#include "Actor.h"

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(Actor& actor, float dt) = 0;
};

class MoveForward : public Command
{
public:
	MoveForward() {};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.MoveForward(dt);
	}
};

class MoveBack : public Command
{
public:
	MoveBack() {};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.MoveBack(dt);
	}
};

class TurnRight : public Command
{
public:
	TurnRight() {};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.TurnRight(dt);
	}
};

class TurnLeft : public Command
{
public:
	TurnLeft() {};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.TurnLeft(dt);
	}
};

class ChangePlayer : public Command
{
public:
	ChangePlayer(bool truth)
	 : m_truth(truth)
	{};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.ChangePlayer(m_truth);
	}
private:
	bool m_truth;

};

class Attack : public Command
{
public: 
	Attack() {};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.Attack(dt);
	}
};

class ChasePlayer : public Command
{
public: 
	ChasePlayer(const Player* player) 
		: m_player(player)
	{};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.Chase(*m_player);
	}
private:
	const Player* m_player;
};

class NavigateToLocation : public Command
{
public: 
	NavigateToLocation(Vec2f target)
		: m_target(target)
	{};

	virtual void execute(Actor& actor, float dt) override
	{
		actor.NavigateTo(m_target);
	}

private:
	Vec2f m_target;
};

class Die : public Command
{
public:
	Die() {};
	virtual void execute(Actor& actor, float dt) override
	{
		actor.Die(dt);
	}
};