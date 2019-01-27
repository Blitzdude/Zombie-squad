#pragma once
#include "Actor.h"
#include "Player.h"
#include "EnemyState.h"

class Enemy : public Actor
{
public:
    Enemy();
    ~Enemy();
   
    void moveLeft(float deltaTime)  override;
    void moveRight(float deltaTime) override;
    void moveUp(float deltaTime)    override;
    void moveDown(float deltaTime)  override;
;
    void move(float tX, float tY, float deltaTime) override;
    void doMove(float deltaTime);

    void update(float deltaTime);

    // Setters/Getters
    EnemyState* getState() const { return m_state; };
    void setState(EnemyState* state) { delete m_state; m_state = state; };
    void setTarget(float x, float y) {targetX = x; targetY = y; };

private:
    EnemyState* m_state;
    Player* m_player;
    float targetX;
    float targetY;
};

