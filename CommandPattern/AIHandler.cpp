#include "AIHandler.h"
#include "Command.h"
#include <iostream>

#include <math.h>

AIHandler::AIHandler(const Player& player)
    : m_player(&player)
{
    //bindCommands();
}

AIHandler::~AIHandler()
{
    // Cleaning, because we are nice
    /*
    delete UpCommand;    UpCommand = nullptr;
    delete DownCommand;  DownCommand = nullptr;
    delete LeftCommand;  LeftCommand = nullptr;
    delete RightCommand; RightCommand = nullptr;
    */
}

Command* AIHandler::handleInput(Enemy &actor)
{
    // see if distance from player is enough to trigger chase
    float pX = m_player->getXPos();
    float pY = m_player->getYPos();
    float eX = actor.getXPos();
    float eY = actor.getYPos();

    // the distance
    float dist = sqrtf((pX - eX)*(pX - eX) + (pY - eY) * (pY - eY));
    
    static const float SIGHT_RANGE = 200.0f; // variable to control enemy sight range
    // get enemys state
    EnemyState* state = actor.getState();
    // else (player is too far) -> tell enemy to change to roam state
    if (dist > SIGHT_RANGE)
    {
        return new RoamCommand();
        //actor.setState(new Roam()); // TODO: this should be a command, not a new state
    }
    // if distance is less then range -> tell enemy to change to chase state,
    else if (dist < SIGHT_RANGE)
    {
        return new ChaseCommand();
       // actor.setState(new Chase()); // TODO: this should be a command, not a new state
    }

    return nullptr;
}

// binds all buttns to Command class object pointers.
void AIHandler::bindCommands()
{
    // TODO: Not needed?
    /*
    UpCommand = new MoveUpCommand();
    DownCommand = new MoveLeftCommand();
    LeftCommand = new MoveDownCommand();
    RightCommand = new MoveRightCommand();
    */
}
