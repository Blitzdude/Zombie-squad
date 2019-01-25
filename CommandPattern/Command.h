#pragma once
#include "Actor.h"

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(Actor& actor, float deltaTime) = 0;

};

class JumpCommand : public Command
{
public:
    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.jump(deltaTime);
    }
};

class MoveUpCommand : public Command
{
    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.moveUp(deltaTime);
    }

};

class MoveDownCommand : public Command
{
    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.moveDown(deltaTime);
    }
};

class MoveLeftCommand : public Command
{
    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.moveLeft(deltaTime);
    }
};

class MoveRightCommand : public Command
{
    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.moveRight(deltaTime);
    }
};

class NothingCommand : public Command
{
    virtual void execute(Actor&, float) override
    {
        // Empty command, do nothing
    }
};