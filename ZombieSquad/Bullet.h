#pragma once
#include "Actor.h"
#include "State.h"

class ZombieSquad;

/**
 * Bullet actor class
 *
 * Whenever bullets overlap an actor, the actor takes damage, if tag of the actor 
 * is different from tag held in the bullet. They are also destroyed if they hit a wall
 * or fly long enough
 */
class Bullet :
	public Actor
{
public:

	/**
	 * Constructs a bullet actor
	 *
	 * 
	 * @param pos bullets starting position coordinates
	 * @param dir direction the bullet travels in radians
	 * @param speed radius size of the bullet
	 * @param lifeTime life time of the bullet in seconds
	 * @param owner Tag of the owning actor (player or zombie)
	 */
	Bullet(const Vec2f& pos, float dir, float speed, float lifeTime, ActorTag owner);
	
	/**
	 * Default destructor of Bullet
	*/
	~Bullet();
	
	/**
	 * Draws the bullet
	 *
	 * Draws the bullet actor. 
	 * @param game reference to the engine instance
	 * @returns no return vale
	 */
	void Draw(ZombieSquad& game) override;

	/**
	 * Updates the bullet
	 *
	 * Updates the bullet according to the delta time. Move the bullet forward
	 * Bullets have a single state that gets updated.
	 * @param dt delta time
	 */
	void Update(float dt) override;

	// Getters

	/// Gets the actor tag of the owner who spawned the bullet
	const ActorTag& GetOwner() { return m_owner; };

	float m_lifeTime;
private:

	ActorTag m_owner;
	BulletState* m_currentState;
	float m_radius;
};

