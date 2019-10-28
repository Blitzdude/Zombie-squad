#pragma once

#include "Actor.h"
#include "State.h"

class ZombieSquad;
class ZombieHandler;

/**
* Short description
*
* Long description
*/
class Zombie : public Actor
{
public:
	/**
	* Short description
	*
	* Long description
	*/
	Zombie(float x, float y, ZombieSquad& game, ZombieHandler& handler);
	~Zombie();

	/**
	* Short description
	*
	* Long description
	*
	* @param game
	*/
	virtual void Draw(ZombieSquad& game) override;

	/**
	* Short description
	*
	* Long description
	* 
	* @param
	*/
	virtual void Update(float dt)  override;

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	virtual void Chase(const Player& player) override;

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	virtual void Die(float dt) override;

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	virtual void Attack(float dt) override;

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	virtual void NavigateTo(const Vec2f& target) override;
	
	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	bool SeesTarget();

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	void SpawnBullet(float dt);

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	void doMove(float dt);

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	void doRoam();

	/**
	* Short description
	*
	* Long description
	*
	* @param
	*/
	void doNavigateTo();

	/**
	* Short description
	*
	* Long description
	*
	* @return
	*/
	Vec2f GetRandomCellLocation();

	/// Sets target for actor
	void SetTarget(const Vec2f& newTarget) { m_target = newTarget; }
private:

	ZombieState* m_currentState; 
	ZombieHandler* m_handler;
	ZombieSquad* m_game; // Needed for bullet spawning
	Vec2f m_target;
};