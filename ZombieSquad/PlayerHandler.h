#pragma once
#include "olcPixelGameEngine.h"
#include "Player.h"
#include <array>
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
	Command* buttonW;
	Command* buttonA;
	Command* buttonS;
	Command* buttonD;
	
	Command* buttonKey1;
	Command* buttonKey2;
	Command* buttonKey3;

	// Command* buttonSpace;
	olc::PixelGameEngine* m_engine;
	std::array<Player*, 3> m_players;
	Player* m_selectedPlayer;
};

