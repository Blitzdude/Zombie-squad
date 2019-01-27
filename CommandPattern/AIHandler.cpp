#include "AIHandler.h"
#include "Enemy.h"
#include "Command.h"
#include <iostream>

#include <math.h>

AIHandler::AIHandler(const Player& player)
    : m_player(&player)
{
    bindCommands();
}

AIHandler::~AIHandler()
{
    // Cleaning, because we are nice
    delete UpCommand;    UpCommand = nullptr;
    delete DownCommand;  DownCommand = nullptr;
    delete LeftCommand;  LeftCommand = nullptr;
    delete RightCommand; RightCommand = nullptr;
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
    
    static const float SIGHT_RANGE = 400.0f; // variable to control enemy sight range
    // get enemys state
    EnemyState* state = actor.getState();
    // else (player is too far) -> tell enemy to change to roam state
    if (state->id == StateID::CHASE && dist > SIGHT_RANGE)
    {
        std::cout << "changing state: Roam\n";
        actor.setState(new Roam());
    }
    // if distance is less then range -> tell enemy to change to chase state,
    else if (state->id == StateID::ROAM && dist < SIGHT_RANGE)
    {
        std::cout << "changing state: Chase\n";
        actor.setState(new Chase());
    }

    // if enemy is in roam state -> check timer
    // if timer -> exceeds alloted time -> change direction randomly

    // if enemy is in chase state -> tell ai to move towards player.

    return nullptr;
}

// binds all buttns to Command class object pointers.
void AIHandler::bindCommands()
{
    // TODO: Not needed?
    UpCommand = new MoveUpCommand();
    DownCommand = new MoveLeftCommand();
    LeftCommand = new MoveDownCommand();
    RightCommand = new MoveRightCommand();
}
