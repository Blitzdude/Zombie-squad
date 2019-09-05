#pragma once
#include "Player.h"
#include "Zombie.h"
#include "Level.h"
#include <vector>

class Command;
class ZombieSquad;
class Player;
class Zombie;
class ZombieSquad;

class ZombieHandler
{
public:
	ZombieHandler(ZombieSquad& engine);
	~ZombieHandler();

	bool Init(Player& p1, Player& p2, Player& p3);

	void HandleZombies(float dt);
	Command* handleInput(Zombie& actor);
	const Player* GetClosestPlayer(Zombie& zombie);
	void AddZombie(Zombie* zombie);
	void RemoveDestroyed();

	bool ZombieSeesTarget(const Vec2f& target, const Zombie& zombie);
private:

	ZombieSquad* m_game;

	std::vector<Zombie*> m_vecZombies;
	Player* m_player1; // TODO: Convert to get a list of players
	Player* m_player2;
	Player* m_player3;
};

