#pragma once

#include "Actor.h"

class Command
{
public:
public:
	virtual ~Command() {}
	virtual void execute(Actor& actor, float deltaTime) = 0;
};

class MoveForward : public Command
{
public:
	MoveForward() {};

	virtual void execute(Actor& actor, float deltaTime) override
	{
		actor.MoveForward(deltaTime);
	}
};

class MoveBack : public Command
{
public:
	MoveBack() {};

	virtual void execute(Actor& actor, float deltaTime) override
	{
		actor.MoveBack(deltaTime);
	}
};

class TurnRight : public Command
{
public:
	TurnRight() {};

	virtual void execute(Actor& actor, float deltaTime) override
	{
		actor.TurnRight(deltaTime);
	}
};

class TurnLeft : public Command
{
public:
	TurnLeft() {};

	virtual void execute(Actor& actor, float deltaTime) override
	{
		actor.TurnLeft(deltaTime);
	}
};

class ChangePlayer : public Command
{
public:
	ChangePlayer() {};

	virtual void execute(Actor& actor, float deltaTime) override
	{
		actor.ChangePlayer();
	}

};

class Attack : public Command
{
public: 
	Attack() {};

	virtual void execute(Actor& actor, float deltaTime) override
	{
		actor.Attack(deltaTime);
	}
};