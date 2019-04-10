#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "PlayerHandler.h"
#include "Player.h"
#include "Command.h"
#include "Level.h"

#include <vector>

/*
ZombieSquad - Project by Joel Känsälä. Public license.
*/

/* STYLE GUIDE
types: (struct, class etc)
	- TypeName
methods, functions: 
	- FunctionName()
variables:
	- type_variableNameAbc
	- type : tells which type the variable is
	- m_=member, c_=constants, p_=parameter
multiple types:
	- mc_= member constant etc.
global constants/Defines:
	- ALL_UPPER_CASE
	
*/

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
	- command object given to actors
	- execute()
*/

/*
Actors::Player, Zombie, Bullet
	- position
	- direction
	- speed
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

/*
Physics
	- list of bodies (if necessary)
	- collision detection
	- collision resolution
		- circle vs circle
		- circle vs edge
	- raycasting
		- raycast result struct
*/
// OTHER
/*
	Math
	- utility 
	- vec2
*/


class ZombieSquad : public olc::PixelGameEngine
{


public:
	ZombieSquad()
		: m_playerHandler(*this)
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

		m_currentLevel = new Level("level3.txt");

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
		vecActors.push_back(new Player(100.0f, 100.0f));
		m_player = vecActors.back();


		// Populate Level with zombies
		/*
			For each open cell far enough away from start
			- Add zombies to unoccupied cells
		*/

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		DoInput(fElapsedTime);
		// Check input
		

		DoUpdate(fElapsedTime);
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

		DoDraw();
		
		return true;
	}

	void DoInput(float fElapsedTime)
	{
		/*
		Check player inputs
		- Keyboard:
		- W/S - Move forward/back
		- A/D - Turn left/right
		- 1,2,3,4 - switch characters
		- Space - fire gun -> create bullets
		*/
		Command* command = m_playerHandler.handleInput();
		if (command)
			command->execute(*m_player, fElapsedTime);

	}

	void DoUpdate(float fElapsedTime)
	{
		
	}

	void DoDraw()
	{
		// Drawing
		/*
			Clear the screen

			Draw tiles
			Draw visibility
			Draw Actors (if in polygon)
		*/

		Clear(olc::BLACK);

		m_currentLevel->DrawLevel(*this);

		for (auto & itr : vecActors)
		{
			itr->Draw(*this);
		}
	}


private:
	PlayerHandler m_playerHandler;
	Actor* m_player;
	std::vector<Actor*> vecActors;
	Level* m_currentLevel;
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