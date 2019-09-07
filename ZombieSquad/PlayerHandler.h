#pragma once
#include "Zombie.h"
#include <array>

class Player;
class Command; // Forward declaration
class ZombieSquad;

class PlayerHandler
{
public:
	PlayerHandler(ZombieSquad& engine);
	~PlayerHandler();

	Command* handleInput();
	void HandlePlayers(float fElapsedTime);
	void bindButtons();
	void addPlayer(Player* player, int index);
	Zombie* GetClosestVisibleZombiePosition(const Player& player); 
	bool PlayerSeesTarget(const Vec2f& targetPos, const Player& player);

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
	const std::vector<Zombie*>* m_zombies;
	std::array<Player*, 3> m_players;
	Player* m_selectedPlayer;
};

