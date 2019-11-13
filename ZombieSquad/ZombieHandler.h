#pragma once
#include "Player.h"
#include "Zombie.h"
#include "Level.h"
#include <vector>

class Command;
class ZombieSquad;
class Player;
class Zombie;
class ZombieSquad;

/**
 * Short description
 *
 * Long description
 *
 */
class ZombieHandler
{
public:
	/**
	* Short description
	*
	* Long description
	*
	* @param engine
	*/
	ZombieHandler(ZombieSquad& engine);

	~ZombieHandler();

	/**
	* Short description
	*
	* Long description
	*
	* @param p1,p2,p3
	*/
	bool Init(Player& p1, Player& p2, Player& p3);

	/**
	* Short description
	*
	* Long description
	*
	* @param dt
	*/
	void HandleZombies(float dt);

	/**
	* Short description
	*
	* Long description
	*
	* @param actor
	*/
	Command* handleInput(Zombie& actor);

	/**
	* Short description
	*
	* Long description
	*
	* @param zombie
	*/
	const Player* GetClosestPlayer(Zombie& zombie);

	/**
	* Short description
	*
	* Long description
	*
	* @param zombie
	*/
	void AddZombie(Zombie* zombie);

	/**
	* Short description
	*
	* Long description
	*
	*/
	void RemoveDestroyed();

	/**
	* Short description
	*
	* Long description
	*
	* @param target
	* @param zombie
	*/
	bool ZombieSeesTarget(const Vec2f& target, const Zombie& zombie);

	/// Gets the vector of zombies.
	const std::vector<Zombie*>& GetZombies() { return m_vecZombies; }
private:

	ZombieSquad* m_game;

	std::vector<Zombie*> m_vecZombies;
	std::vector<Player*> m_vecPlayers;
};

