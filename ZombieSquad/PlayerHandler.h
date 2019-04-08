#pragma once
#include "olcPixelGameEngine.h"

class Command; // Forward declaration

class PlayerHandler
{
public:
	PlayerHandler(olc::PixelGameEngine& engine);
	~PlayerHandler();

	Command* handleInput();
	void bindButtons();

private:
	Command* buttonW;
	Command* buttonA;
	Command* buttonS;
	Command* buttonD;
	// Command* buttonSpace;

	olc::PixelGameEngine* m_engine;
};

