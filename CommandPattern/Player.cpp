#include "Player.h"

Player::Player()
{
    setXPos(100.0f);
    setYPos(100.0f);
}

Player::~Player()
{
}

void Player::jump(float deltaTime)
{
    this->moveUp(deltaTime);
}

void Player::moveLeft(float deltaTime)
{
    setXPos(getXPos() - 100.0f*deltaTime);
}

void Player::moveRight(float deltaTime)
{
    setXPos(getXPos() + 100.0f*deltaTime);
}

void Player::moveUp(float deltaTime)
{
    setYPos(getYPos() - 100.0f*deltaTime);
}

void Player::moveDown(float deltaTime)
{
    setYPos(getYPos() + 100.0f*deltaTime);
}
