#pragma once
#include "Actor.h"


class Command
{
public:
    virtual ~Command() {}
    virtual void execute(Actor& actor, float deltaTime) = 0;
};

/*
class JumpCommand : public Command
{
public:

    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.jump(deltaTime);
    }
};
*/

class Move : public Command
{
    Move() = delete; // No default construction
    Move(float tX, float tY)
        : targetX(tX)
        , targetY(tY)
    {};


    virtual void execute(Actor& actor, float deltaTime) override
    {
        actor.move(targetX, targetY, deltaTime);
    }
    // TODO: Replace with A-star path when switching
    float targetX;
    float targetY;
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

class ChaseCommand : public Command
{
 
 void execute(Actor& actor, float deltaTime) override 
    {
        // TODO: check if actor has chase command. Maybe check if is type enemy?
        actor.chase();
    };
    
};

class RoamCommand : public Command
{
    void execute(Actor& actor, float deltaTime) override 
    {
        // TODO: check if actor has roam command. Maybe check if is type enemy?
        actor.roam();
    };

};