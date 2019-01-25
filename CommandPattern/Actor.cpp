#include "Actor.h"
#include <iostream>

Actor::Actor()
    : m_xPos(0.0f)
    , m_yPos(0.0f)
{
}

Actor::~Actor()
{
}

void Actor::jump()
{
    std::cout << "jumping\n";
}

void Actor::moveLeft()
{
    std::cout << "moving left\n";
}

void Actor::moveRight()
{
    std::cout << "moving right\n";
}

void Actor::moveUp()
{
    std::cout << "moving up\n";
}

void Actor::moveDown()
{
    std::cout << "moving down\n";
}
