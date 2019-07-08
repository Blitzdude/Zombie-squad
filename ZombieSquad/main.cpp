#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "PlayerHandler.h"
#include "Player.h"
#include "Command.h"
#include "Level.h"
#include "Physics.h"

#include <vector>
#include <iostream>

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
		std::vector<std::pair<Actor*, Edge*>> vec_collidingActors; // Container to hold colliding actors and edges
		m_isColliding = false;
		for (auto& itr : vecActors)
		{
			// if actor is colliding with edge, draw a circle around it
			for (auto& edge : m_currentLevel->GetEdges())
			{
				if (m_physics.isColliding(*itr, edge) > 0.0f)
				{
					vec_collidingActors.emplace_back(itr, new Edge(edge));
					m_isColliding = true;
				}

			}
		}

		// For each item in CollidingActors- vector container, resolve collisions
		for (auto p : vec_collidingActors)
		{
			m_physics.resolveEdgeCircle(p.first, p.second->normal, 0.1f);
		}
		vec_collidingActors.clear();
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


		// Debug: Draw the perpendicular distances from actor to edges
		for (auto& act : vecActors)
		{
			for (auto& edge : m_currentLevel->GetEdges())
			{
				// Check that line formed by velocity vector, intersects with line segment
				float fLineX1 = edge.end.x - edge.start.x;
				float fLineY1 = edge.end.y - edge.start.y;

				float fLineX2 = act->GetX() - edge.start.x;
				float fLineY2 = act->GetY() - edge.start.y;

				float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

				// This is nifty - It uses the DP of the line segment vs the line to the object, to work out
				// how much of the segment is in the "shadow" of the object vector. The min and max clamp
				// this to lie between 0 and the line segment length, which is then normalised. We can
				// use this to calculate the closest point on the line segment
				float t = std::max(0.0f, std::min(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;

				// Which we do here
				float fClosestPointX = edge.start.x + t * fLineX1;
				float fClosestPointY = edge.start.y + t * fLineY1;
				DrawLine(act->GetX(), act->GetY(), fClosestPointX, fClosestPointY, olc::MAGENTA, 0xFF00FF00);
				FillCircle(fClosestPointX, fClosestPointY, 3, olc::MAGENTA);
			}
		}

		// m_currentLevel->DrawLevel(*this);
		m_currentLevel->DrawPolyMap(*this);

		for (auto & itr : vecActors)
		{
			itr->Draw(*this);
			// if actor is colliding with edge, draw a circle around it
			if (m_isColliding)
			{
				DrawCircle(itr->GetX(), itr->GetY(), itr->GetRadius() + 2, olc::CYAN);
			}
		}

		
	}


private:
	PlayerHandler m_playerHandler;
	Actor* m_player;
	std::vector<Actor*> vecActors;
	Level* m_currentLevel;
	Physics m_physics;

	bool m_isColliding = false;
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