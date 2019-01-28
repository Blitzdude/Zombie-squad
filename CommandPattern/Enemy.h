#pragma once
#include "Actor.h"
#include "Player.h"
#include "EnemyState.h"

class Enemy : public Actor
{
public:
    Enemy(const Player& player, float x, float y);
    ~Enemy() {};

    void move(float tX, float tY, float deltaTime) override;
    void chase() override;
    void roam() override;
    void doMove(float deltaTime);
    void update(float deltaTime);

    // Setters/Getters
    EnemyState* getState() const { return m_state; }; // TODO: maybe not needed?
    void setState(EnemyState* state) { delete m_state; m_state = state; }; // TODO: Maybe not needed, when removed from AiHandler
    void setTarget(float x, float y) { targetX = x; targetY = y; };

private:
    EnemyState* m_state;
    const Player* m_player;
    float targetX;
    float targetY;
};

