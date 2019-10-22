#define OLC_PGE_APPLICATION
#include "ZombieSquad.h"
#include <vector>
#include <iostream>

/*
ZombieSquad - Project by Joel Känsälä. Public license.
*/

/* STYLE GUIDE
types: (struct, class etc)
	- TypeName (PascalCase)
methods, functions: 
	- FunctionName()
variables:
	- type_variableNameAbc
	- type : tells which type the variable is
	- m_=member, c_=constants, p_=parameter
multiple types:
	- mc_= member constant etc.
global constants/Defines: (ANGRY_SNAKE_CASE)
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


int main(int argc, char* argv[])
{
	std::string str = argv[1];
	bool startWithDevMode = argc > 1 && str == "devmode" ? true : false;
	ZombieSquad game(startWithDevMode);
	if (game.Construct(1200, 800, 1, 1))
	{
		game.Start();
	}
	return 0;
}