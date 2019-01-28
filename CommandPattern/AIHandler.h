#pragma once
#include "Player.h"
#include "Enemy.h"
class Command;

class AIHandler
{
public:
    AIHandler(const Player& player);
    ~AIHandler();

    Command* handleInput(Enemy& actor);
    void bindCommands();

private:
/*
    Command* UpCommand;
    Command* DownCommand;
    Command* LeftCommand;
    Command* RightCommand;
*/
    const Player* m_player; // AI handler only reads from player, and not change it's locations
};

