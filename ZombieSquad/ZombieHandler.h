#pragma once
#include "Player.h"
#include "Zombie.h"
#include <vector>

class Command;

class ZombieHandler
{
public:
	ZombieHandler(Player& p1, Player& p2, Player& p3);
	~ZombieHandler();

	Command* handleInput(Zombie& actor);
	const Player* GetClosestPlayer(Zombie& zombie);

private:
	Player* m_player1; // AI handler only reads from player, and not change it's locations
	Player* m_player2;
	Player* m_player3;
};

