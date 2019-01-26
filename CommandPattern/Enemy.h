#pragma once
#include "Actor.h"

class Enemy : public Actor
{
public:
    Enemy();
    ~Enemy();

    void moveLeft(float deltaTime)  override;
    void moveRight(float deltaTime) override;
    void moveUp(float deltaTime)    override;
    void moveDown(float deltaTime)  override;
};

