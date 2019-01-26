#include "Enemy.h"

Enemy::Enemy()
{
    setXPos(200.0f);
    setYPos(100.0f);
}

Enemy::~Enemy()
{
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
