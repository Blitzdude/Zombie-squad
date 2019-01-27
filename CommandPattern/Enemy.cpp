#include "Enemy.h"

Enemy::Enemy()
{
    m_state = new Roam();
    setXPos(200.0f);
    setYPos(100.0f);
}

Enemy::~Enemy()
{
}

void Enemy::move(float tX, float tY, float deltaTime)
{
    // move in the direction of the target
    /*
    0,0-w,0
    |     |
    0,h-w,h
    */
    
    // TODO: Currently called every update, actor should prob handle its own moving
    // something like doMove() in update, if target hasn't been reached, 

    if (targetX != tX)
        targetX = tX;
    if (targetY != tY)
        targetY = tY;
/*
    if (getXPos() < targetX)
        moveRight(deltaTime); // replace with straight math?
    else if (getXPos() > targetX)
        moveLeft(deltaTime); // replace with straight math?
    if (getYPos() < targetY)
        moveDown(deltaTime); // replace with straight math?
    else if (getYPos() > targetY)
        moveUp(deltaTime); // replace with straight math?
*/
}

void Enemy::doMove(float deltaTime)
{
    if (getXPos() < targetX)
        moveRight(deltaTime); // replace with straight math?
    else if (getXPos() > targetX)
        moveLeft(deltaTime); // replace with straight math?
    if (getYPos() < targetY)
        moveDown(deltaTime); // replace with straight math?
    else if (getYPos() > targetY)
        moveUp(deltaTime); // replace with straight math?
}

void Enemy::update(float deltaTime)
{
    m_state->update(*this, deltaTime);
}

void Enemy::moveLeft(float deltaTime)
{
    setXPos(getXPos() - 10.0f*deltaTime);
}

void Enemy::moveRight(float deltaTime)
{
    setXPos(getXPos() + 10.0f*deltaTime);
}

void Enemy::moveUp(float deltaTime)
{
    setYPos(getYPos() - 10.0f*deltaTime);
}

void Enemy::moveDown(float deltaTime)
{
    setYPos(getYPos() + 10.0f*deltaTime);
}
