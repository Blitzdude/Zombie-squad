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
	virtual void Update(float dt) override;

	virtual void MoveForward(float dt)    override;
	virtual void MoveBack(float dt)		  override;
	virtual void TurnRight(float dt)	  override;
	virtual void TurnLeft(float dt)		  override;
	virtual void Attack(float dt)		  override;
	virtual void Die(float dt)			  override;
	virtual void ChangePlayer(bool truth) override;

	PlayerState* GetCurrentState() const { return m_currentState; };
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

