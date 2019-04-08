#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// CLASSES
/*
Level
	- holds all data for the map
	- Tile data
	- Edge data
	- Handles pathfindng from one point to another (A* star)
*/

/*
Command
	- part of command pattern
	- command object given
	- execute()
*/

/*
Actors::Player, Zombie, Bullet
	- position
	- direction
	- attack()
	- move()
	- have states
*/

/*
State
	- current state
	- what actor is currently doing
	- has entry, current and leave actions
*/


/*
Handler::Input, ZombieAI, CharacterAI
	- Input: send commands based on input.
	- ZombieAI: tell zombies to do stuff
	- CharacterAI: tell characters not beign controlled to do stuff
*/

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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		// Check input
		/*
			Check player inputs
			- Keyboard:
			- W/S - Move forward/back
			- A/D - Turn left/right
			- 1,2,3,4 - switch characters
			- Space - fire gun -> create bullets
		*/

		// Update 
		/*
			Check victory condition
			- All player characters are in the goal cell
			Check Collisions
			Resolve collisions
			- Move zombies and characters away from walls
			- destroy bullets, when they touch something
			Move Actors (zombies, characters, bullets)
			Calculate visibility polygons

			
			
		*/

		// Drawing
		/*
			Draw tiles
			Draw visibility
			Draw Actors (if in polygon)
		*/
		return true;
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