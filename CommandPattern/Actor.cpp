#include "Actor.h"
#include <iostream>

void Actor::jump(float deltaTime)
{
    std::cout << "jumping\n";
}

void Actor::move(float tX, float tY, float deltaTime)
{
    std::cout << "moving to " << tX << " " << tY << "\n";
}

void Actor::moveLeft(float deltaTime)
{
    std::cout << "moving left\n";
}

void Actor::moveRight(float deltaTime)
{
    std::cout << "moving right\n";
}

void Actor::moveUp(float deltaTime)
{
    std::cout << "moving up\n";
}

void Actor::moveDown(float deltaTime)
{
    std::cout << "moving down\n";
}
