#pragma once
#include "olcPixelGameEngine.h"
#include "Zombie.h"
#include "Bullet.h"
#include "ZombieHandler.h"
#include "PlayerHandler.h"
#include "Command.h"
#include "Level.h"

#include <vector>
#include <iostream>

/**
* The main game class
* 
* ZombieSquad class holds all the necessary logic to run the game. It creates the level on startup
* and updates the Actors, handlers and tracks logic. ZombieSquad inherits from olc::PixelGameEngine,
* giving it access to drawing and input functions. 
*/
class ZombieSquad : public olc::PixelGameEngine
{

public:
	ZombieSquad(bool devmode = false);
	
	/**
	* Creates and initializes all necessary variables for the game 
	*
	* Holds all the logic considering creation and initialization of objects and game states
	* Called only once. Overrides function from the PixelGameEngine class
	*
	* @return returns true, if no errors were encountered during initialization. False
	* if something went wrong.
	*
	*/
	bool OnUserCreate() override;

	/**
	 * Updates the game
	 *
	 * Holds all game logic and updates every frame. This is the location of the main game loop
	 * 
	 * OnUserUpdate calls DoInput, DoUpdate and DoDraw methods to update different parts of the
	 * logic. Overrides function from the PixelGameEngine class
	 *
	 * @param fElapsedTime time elapsed between update frames.
	 * @return If returns true, game continues updating. returning false tells game to quit
	 */
	bool OnUserUpdate(float fElapsedTime) override;

	/**
	 * Updates user inputs and AI Handlers
	 *
	 * Updates the handlers and player input. Esc-key checking is done
	 * so we can quit even if the game is already over.
	 *
	 * @param fElapsedTime time elapsed between update frames.
	 */
	void DoInput(float fElapsedTime);

	/**
	 * Updates Game, Actors and physics
	 *
	 * Updates the game logic, actors and Physics. Also creation and
	 * deletion of actors is done here. 
	 *
	 * @param fElapsedTime time elapsed between update frames.
	 */
	void DoUpdate(float fElapsedTime);

	/**
	 * Draws the Game, Actors and level
	 *
	 * Draws the game every frame. All functions for drawing are inherited
	 * from PixelGameEngine. 
	 *
	 * @param fElapsedTime time elapsed between update frames.
	 */
	void DoDraw();

	/**
	 * Checks if the player has won
	 *
	 * Checks if all the survivors still alive are on the goal tile
	 *
	 * @param fElapsedTime time elapsed between update frames.
	 */
	bool CheckVictory();
	bool GameIsOver();

	Player* SpawnPlayer(float xPos, float yPos, float dir, int playerNum, ZombieSquad& game, PlayerHandler& playerHandler, float offset = 0.0f, bool startingPlayer = false);
	void SpawnZombie(int x, int y, float offset = 0.0f);
	void SpawnBullet(const Vec2f& pos, float dir, float lifetime, float speed, ActorTag tag);

	void AddActors(); 

	// Getters
	const std::vector<Actor*>& GetActors() { return vecActors; };

	ZombieHandler m_zombieHandler;
	PlayerHandler m_playerHandler;

	Level* m_currentLevel;

	bool DevMode() { return m_devMode; };

private:

	bool m_devMode;
	std::vector<Actor*> vecActors;
	std::vector<Actor*> vecActorsToAdd;

	/// turns on devmode for additional drawing
	bool m_isWin;
	bool m_isGameOver;
	bool m_isRunning;
};

