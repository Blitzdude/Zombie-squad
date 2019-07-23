#pragma once
#include "olcPixelGameEngine.h"

#include <array>

class Player;
class Command; // Forward declaration

class PlayerHandler
{
public:
	PlayerHandler(olc::PixelGameEngine& engine);
	~PlayerHandler();

	Command* handleInput();
	void HandlePlayers(float fElapsedTime);
	void bindButtons();
	void addPlayer(Player* player, int index);

private:
	Command* buttonW; // forward
	Command* buttonA; // back
	Command* buttonS; // turn right
	Command* buttonD; // turn left
	
	Command* buttonKey1; // Change to player 1
	Command* buttonKey2; // Change to player 2
	Command* buttonKey3; // Change to player 3
	Command* buttonSpace; // Fire Gun

	olc::PixelGameEngine* m_engine;
	std::array<Player*, 3> m_players;
	Player* m_selectedPlayer;
};

