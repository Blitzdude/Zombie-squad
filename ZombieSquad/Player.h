#pragma once
#include "Actor.h"
#include "State.h"

class ZombieSquad;
class Bullet;
class PlayerState;
class PlayerHandler;
class Bullet;

/**
* Player characters
*
* Actors are controlled by the player and are updated by the player handler.
* Is inherited from Actor class
*
*/
class Player : public Actor
{
public:
	Player(float x, float y, float dir, int playerNum, ZombieSquad& game, PlayerHandler& player_handler, bool startingPlayer = false);
	~Player();

	/**
	* Draws the player
	*
	* Used to draw the player. Overrides Draw from Actor.
	* 
	* @param game Reference to game for drawing purposes
	*/
	virtual void Draw(ZombieSquad& game) override;

	/**
	* Updates the player
	* 
	* Updates the current state of the Player object
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void Update(float dt) override;

	/**
	* Moves the Player forward
	*
	* Moves the Player object in the direction it's facing
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void MoveForward(float dt)    override;

	/**
	* Moves the Player backwards
	*
	* Moves the Player object away from the direction it's facing
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void MoveBack(float dt)		  override;

	/**
	* Turns the Player right
	*
	* Turns the Player right
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void TurnRight(float dt)	  override;

	/**
	* Turns the Player left
	*
	* Turns the Player left
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void TurnLeft(float dt)		  override;

	/**
	* Makes player attack
	*
	* The Player shoots a Bullet in the direction it's facing.
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void Attack(float dt)		  override;

	/**
	* Makes Player die
	*
	* Makes Player go to Dying State where it cannot die.
	* @param dt deltaTime value (Time between frames)
	*/
	virtual void Die(float dt)			  override;

	/**
	* Changes the controlled Player character.
	*
	* Changes the Player character state to Controlled or Watching depending
	* the boolean
	* @param truth If true this becomes the controlled player. 
	*/
	virtual void ChangePlayer(bool truth) override;

	/// Gets the pointer to the current actors state.
	PlayerState* GetCurrentState() const { return m_currentState; };
	/// Gets the pointer to the closest Zombie object.
	const Zombie* GetVisibleZombie();

	// void SetWeapon(Bullet* bullet) { m_gun = bullet; };

private:
	PlayerState* m_currentState;
	PlayerHandler* m_handler;
	ZombieSquad* m_game; // Needed for bullet spawning
	float m_fireRate;
	float m_lastTimeFired;
	int m_playerNumber;

	//Bullet* m_gun; // Which kind of bullets to spawn when firing // Not implemented
};

