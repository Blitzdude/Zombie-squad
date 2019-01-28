#include "Enemy.h"
#include <math.h>

Enemy::Enemy(const Player& player, float x, float y)
    : targetX(x)
    , targetY(y)
{
    m_state = new Roam();
    m_player = &player; // Enemy should only read from player, and not change anything in its states
    setXPos(x);
    setYPos(y);
}


void Enemy::move(float tX, float tY, float deltaTime) // TODO: Not needed ???
{
    // move in the direction of the target
    /*
    0,0-w,0
    |     |
    0,h-w,h
    */

    if (targetX != tX)
        targetX = tX;
    if (targetY != tY)
        targetY = tY;
}

void Enemy::chase()
{
    if (m_state->getStateID() != StateID::CHASE)
        setState(new Chase(*m_player));
}

void Enemy::roam()
{
    if (m_state->getStateID() != StateID::ROAM)
        setState(new Roam());
}

void Enemy::doMove(float deltaTime)
{
    // get vector to target
    float vecX = targetX - getXPos();
    float vecY = targetY - getYPos();
    float length = sqrtf(vecX*vecX + vecY*vecY);

    // normalize vector
    vecX /= length;
    vecY /= length;

    if (length > 0.1f)
    {
       setXPos(getXPos() + vecX * 50.0f * deltaTime);
       setYPos(getYPos() + vecY * 50.0f * deltaTime);
    }
}

void Enemy::update(float deltaTime)
{
    m_state->update(*this, deltaTime);
}
