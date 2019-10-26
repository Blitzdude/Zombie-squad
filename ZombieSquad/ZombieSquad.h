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
	 */
	void DoDraw();

	/**
	 * Checks if the player has won
	 *
	 * Checks if all the survivors still alive are on the goal tile
	 */
	bool CheckVictory();

	/**
	 * Checks if game is over
	 *
	 * Checks if the game is over. The game is over if all the players are dead or all alive players are on the goal tile
	 *
	 * @return returns true if players are dead or all alive players are on the goal. Otherwise returns false, meaning the game should continue to run
	 */
	bool GameIsOver();

	/**
	 * Spawns a Plyaer actor
	 *
	 * Spawns a Player actor. The Player is first created and given to the PlayerHandler object. The actor is also added to the actors vector for updating
	 * 
	 * @param xPos,yPos Coordinate position to spawn the actor.
	 * @param dir Starting facing direction.
	 * @param playerNum The players number for selection.
	 * @param game Reference to the main game.
	 * @param offset Offset position to spawn the actor, so they do not start on top of each other.
	 * @param startingPlayer Boolean for if this is the player that begins controlled by user. 
	 * @return returns pointer to the created player.
	 */
	Player* SpawnPlayer(float xPos, float yPos, float dir, int playerNum, ZombieSquad& game, float offset = 0.0f, bool startingPlayer = false);

	/**
	 * Spawns a Zombie actor
	 *
	 * Spawns a Zombie actor. The Zombie is first created and given to the Zombiehandler object. The actor is also added to the actors vector for updating.
	 *
	 * @param x,y Coordinate position to spawn the actor.
	 * @param offset offset for the position to spawn in.
	 */
	void SpawnZombie(int x, int y, float offset = 0.0f);

	/**
	 * Spawns a Bullet actor
	 *
	 * Spawns a Bullet actor. The Bullet is first created and given to the Zombiehandler object. The actor is also added to the actors vector for updating.
	 * As Bullets can be spawned by Actors, we need to make sure we do not add actors during updating the Actors, as this invalidates any iterators.
	 * Therefore we first add the bullet to vecActorsToAdd vector. All of these actors are added to the game by the AddActors method after updating the Actors is done.
	 *
	 * @param pos Coordinate position to spawn the actor.
	 * @param dir Starting facing direction.
	 * @param lifetime How long will the bullet fly forward in seconds
	 * @param speed Movement speed of the bullet.
	 * @param tag Tag of the owning actor.
	 * @return returns pointer to the created player.
	 */
	void SpawnBullet(const Vec2f& pos, float dir, float lifetime, float speed, ActorTag tag);

	/**
	* Adds actors to the actors list
	*
	* Pushes the Actors spawned by Players and Zombies to the vector of actors. 
	* This is done separately, because we cannot add actors while we are iterating over them
	*/
	void AddActors(); 

	
	/// Gets the vector of Actors
	const std::vector<Actor*>& GetActors() { return vecActors; };
	/// Returns if we are in developer mode
	bool DevMode() { return m_devMode; };

	ZombieHandler m_zombieHandler;
	PlayerHandler m_playerHandler;

	Level* m_currentLevel;
private:

	bool m_devMode;
	std::vector<Actor*> vecActors;
	std::vector<Actor*> vecActorsToAdd;

	/// turns on devmode for additional drawing
	bool m_isWin;
	bool m_isGameOver;
	bool m_isRunning;
};

