#include "EnemyState.h"
#include "Enemy.h"
#include <random>

void EnemyState::enter(Enemy & enemy)
{
    // base, do nothing
}

void EnemyState::update(Enemy & enemy, float deltaTime)
{
    // base, do nothing
}

void EnemyState::exit(Enemy & enemy, float deltaTime)
{
    // base, do nothing
}

void Chase::enter(Enemy& enemy)
{
    // change graphics, play sound etc..
    std::cout << "enter: Zombie says 'Braainzz!!'\n";
}

void Chase::update(Enemy & enemy, float deltaTime)
{
    enemy.setTarget(m_player->getXPos(), m_player->getYPos());
    enemy.doMove(deltaTime);
}

void Roam::update(Enemy & enemy, float deltaTime)
{
    // add to timer
    m_timer += deltaTime;
    static const float TIME_UNTIL_CHANGE_DIR = 3.0f; 
    if (m_timer >= TIME_UNTIL_CHANGE_DIR)
    {
        m_timer = 0.0f;
        // randomize direction
        float rX = ((rand() % (2*100)) - 100) / 100.0f; // returns value between -1.0f / 1.0f 
        float rY = ((rand() % (2*100)) - 100) / 100.0f; // returns value between -1.0f / 1.0f
        enemy.setTarget(enemy.getXPos() + rX * 100.0f, enemy.getYPos() + rY * 100.0f); // TODO: replace 100.0f with enemy-speed
    }
    enemy.doMove(deltaTime);

}
