#pragma once
#include "Player.h"
#include "Zombie.h"
#include <vector>

class Command;

class ZombieHandler
{
public:
	ZombieHandler(const Player& p1, const Player& p2, const Player& p3);
	~ZombieHandler();

	Command* handleInput(Zombie& actor);

private:
	const Player* m_player1; // AI handler only reads from player, and not change it's locations
	const Player* m_player2;
	const Player* m_player3;
};

