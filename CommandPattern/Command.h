#pragma once
#include "Actor.h"

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(Actor& actor) = 0;

};

class JumpCommand : public Command
{
public:
    virtual void execute(Actor& actor) override
    {
        actor.jump();
    }
};

class MoveUpCommand : public Command
{
    virtual void execute(Actor& actor) override
    {
        actor.moveUp();
    }

};

class MoveDownCommand : public Command
{
    virtual void execute(Actor& actor) override
    {
        actor.moveDown();
    }
};

class MoveLeftCommand : public Command
{
    virtual void execute(Actor& actor) override
    {
        actor.moveLeft();
    }
};

class MoveRightCommand : public Command
{
    virtual void execute(Actor& actor) override
    {
        actor.moveRight();
    }
};

class NothingCommand : public Command
{
    virtual void execute(Actor& actor) override
    {
        // Empty command, do nothing
    }
};