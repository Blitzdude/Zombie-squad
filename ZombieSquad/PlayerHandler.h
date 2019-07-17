#pragma once
#include "olcPixelGameEngine.h"
#include "Player.h"
class Command; // Forward declaration

class PlayerHandler
{
public:
	PlayerHandler(olc::PixelGameEngine& engine);
	~PlayerHandler();

	Command* handleInput();
	void HandlePlayers(float fElapsedTime);
	void bindButtons();
	void addPlayer(Player* player);

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
	std::vector<Player*> m_players;
	Player* m_selectedPlayer;
};

