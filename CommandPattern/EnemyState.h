#pragma once
#include "Command.h"

#include <iostream>

class Player;
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

    virtual void enter(Enemy& enemy, float deltaTime);
    virtual void update(Enemy& enemy, float deltaTime);
    virtual void exit(Enemy& enemy, float deltaTime);

    const StateID& getStateID() const { return id; };

protected:
    StateID id; // TODO: make getters for id, this breaks encapsulation
};

// TODO: send chase target through reference to actor in constructor
class Chase : public EnemyState
{
public:
    Chase(const Player& player)
        : m_player(&player)
    {
        id = StateID::CHASE;    
    };

    ~Chase() 
    {
        // pointer to player is not owned by Chase, so don't delete the object
        m_player = nullptr; 
    };

    void update(Enemy& enemy, float deltaTime) override;
private:
    const Player* m_player;
};

class Roam : public EnemyState
{
public:
    Roam()
        : m_timer(0.0f)
    {
        id = StateID::ROAM;
    };
    ~Roam() { std::cout << "[Roam]: destructor\n"; };

    void update(Enemy& enemy, float deltaTime) override;

    float m_timer;
};
