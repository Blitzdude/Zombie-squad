#pragma once
#include "Zombie.h"
#include <array>

class Player; // Forward declaration
class Command; 
class ZombieSquad;
class ZombieHandler;

class PlayerHandler
{
public:
	friend Player;

	PlayerHandler(ZombieSquad& engine);
	~PlayerHandler();

	bool Init(ZombieHandler& zombieHandler);

	Command* handleInput();
	void HandlePlayers(float fElapsedTime);
	void bindButtons();
	void addPlayer(Player* player, int index);
	const Zombie* GetClosestVisibleZombiePosition(const Player& player);
	bool PlayerSeesTarget(const Vec2f& targetPos, const Player& player);

	const std::array<Player*, 3> GetPlayers() { return m_players; }

private:
	Command* buttonW; // forward
	Command* buttonA; // back
	Command* buttonS; // turn right
	Command* buttonD; // turn left
	
	Command* buttonKey1; // Change to player 1
	Command* buttonKey2; // Change to player 2
	Command* buttonKey3; // Change to player 3
	Command* buttonSpace; // Fire Gun
	Command* buttonChangeTrue;
	Command* buttonChangeFalse;

	ZombieSquad* m_game;
	ZombieHandler* m_zombieHandler;
	std::array<Player*, 3> m_players;
	Player* m_selectedPlayer;
};

