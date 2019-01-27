#pragma once
#include "Command.h"

class Enemy;

enum class StateID
{
    CHASE,
    ROAM,
    NONE
};

class EnemyState
{
public:
    EnemyState()
        : id(StateID::NONE)
    {}

    virtual ~EnemyState() {};

    virtual void update(Enemy& enemy, float deltaTime);

    StateID id; // TODO: make setters/getters for id, this breaks encapsulation
};

// TODO: send chase target through reference to actor in constructor
class Chase : public EnemyState
{
public:
    Chase()
    {
        id = StateID::CHASE;    
    };

    ~Chase() {};

    void update(Enemy& enemy, float deltaTime) override;
};

class Roam : public EnemyState
{
public:
    Roam()
        : m_timer(0.0f)
    {
        id = StateID::ROAM;
    };
    ~Roam() {};

    void update(Enemy& enemy, float deltaTime) override;

    float m_timer;


};
