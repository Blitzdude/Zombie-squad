#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class ZombieSquad : public olc::PixelGameEngine
{
public:
	ZombieSquad()
	{
		sAppName = "ZombieSquad";
	}

	bool OnUserCreate() override
	{
		// Load Level data
		/*
			world width, height, blockwidth
			start location
			goal location
		*/

		// Generate Edge data from tile map
		/*
			see lineOfSight demo
		*/

		// Put player characters into the game
		/*
			Set xy-coordinates
			Set size
			Give control to player
		*/

		// Populate Level with zombies
		/*
			For each open cell far enough away from start
			- Add zombies to unoccupied cells
		*/

	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Check victory condition
		/*
			All player characters are in the goal cell
		*/

		// Check input
		/*
			Check player inputs
			- Keyboard:
			- W/S - Move forward/back
			- A/D - Turn left/right
			- 1,2,3,4 - switch characters
			- Space - fire gun
		*/

		// Update 
	}

private:

};


int main()
{
	ZombieSquad game;
	if (game.Construct(800, 600, 1, 1))
	{
		game.Start();
	}
	return 0;
}