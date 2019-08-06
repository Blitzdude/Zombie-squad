#pragma once


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
	// Vec2f GetClosestZombiePosition(const Vec2f& player); TODO:: not implemented

private:
	Command* buttonW; // forward
	Command* buttonA; // back
	Command* buttonS; // turn right
	Command* buttonD; // turn left
	
	Command* buttonKey1; // Change to player 1
	Command* buttonKey2; // Change to player 2
	Command* buttonKey3; // Change to player 3
	Command* buttonSpace; // Fire Gun

	ZombieSquad* m_game;
	std::array<Player*, 3> m_players;
	Player* m_selectedPlayer;
};

