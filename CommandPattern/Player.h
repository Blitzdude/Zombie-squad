#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
    Player();
    ~Player();

    void jump(float deltaTime)      override;
    void moveLeft(float deltaTime)  override;
    void moveRight(float deltaTime) override;
    void moveUp(float deltaTime)    override;
    void moveDown(float deltaTime)  override;

private:

};


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
    setXPos(getXPos() - 1000.0f*deltaTime);
}

void Player::moveRight(float deltaTime)
{
    setXPos(getXPos() + 1000.0f*deltaTime);
}

void Player::moveUp(float deltaTime)
{
    setYPos(getYPos() - 1000.0f*deltaTime);
}

void Player::moveDown(float deltaTime)
{
    setYPos(getYPos() - 1000.0f*deltaTime);
}
