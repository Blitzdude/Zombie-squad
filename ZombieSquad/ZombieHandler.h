#pragma once
#include "Player.h"
#include "Zombie.h"
#include "Level.h"
#include <vector>

class Command;

class ZombieHandler
{
public:
	ZombieHandler();
	~ZombieHandler();

	bool Init(Player& p1, Player& p2, Player& p3, const Level &lev);

	void HandleZombies(float dt);
	Command* handleInput(Zombie& actor);
	const Player* GetClosestPlayer(Zombie& zombie);
	void AddZombie(Zombie* zombie);
	void RemoveDestroyed();

	bool ZombieSeesTarget(const Vec2f& target, const Zombie& zombie);
private:

	const Level* m_level;

	std::vector<Zombie*> m_vecZombies;
	Player* m_player1; // TODO: Convert to get a list of players
	Player* m_player2;
	Player* m_player3;
};

