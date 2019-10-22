#pragma once
#include "olcPixelGameEngine.h"
#include "Vec2.h"

class ZombieSquad;
class Player;


/**
 * ActorTag differenciates the types of actors
 * this is required for determening overlap results in physics
 */
enum class ActorTag {
	NONE = 0,
	PLAYER,
	ZOMBIE,
	BULLET
};


/** 
 * Abstract base class for all actors
 *
 * Actor acts as a parent class for Player, Zombie and Bullet classes.
 * As this class is abstract, no objects can or should be instanciated
 * from this class
 */
class Actor
{
public:
	Actor();
	virtual ~Actor();
	/**
	 *  Draws the actor
	 *
	 *  Abstract method used to draw the actor. Must be implemented by children
	 *
	 *  @param game reference to
	 *  @returns no return value
	 */
	virtual void Draw(ZombieSquad& game) = 0;
	
	/**
	 *  Updates the actor
	 *
	 *  Abstract method used to update the actor. Must be implemented by children
	 *
	 *  @param dt delta time: time elapsed since previous update
	 *  @see DoUpdate()
	 */
	virtual void Update(float dt) = 0;
	
	/**
	 *  Moves a player actor forward
	 *
	 *  Command moves player actor in the direction its currently facing
	 *  overriden by child actor objects
	 *  @param dt delta time: time elapsed since the previous update.
	 *  @see Command()
	 */
	virtual void MoveForward(float dt) {};
	
	/**
	 *  Moves a player actor backwards
	 *
	 *  Command moves player actor directly away from the direction its currently facing
	 *  overriden by child actor objects
	 *  @param dt delta time: time elapsed since the previous update.
	 *  @see Command()
	 */
	virtual void MoveBack(float dt)	{};
	
	/**
	 *  Turns player actor right
	 *
	 *  Command turns player actor clockwise to the right
	 *  overriden by child actor objects
	 *  @param dt delta time: time elapsed since the previous update.
	 *  @see Command()
	 */
	virtual void TurnRight(float dt)  {};
	
	/**
	 *  Turns player actor left
	 *
	 *  Command turns player actor counter-clockwise to the left
	 *  overriden by child actor objects
	 *  @param dt delta time: time elapsed since the previous update.
	 *  @see Command()
	 */
	virtual void TurnLeft(float dt)   {};
	
	/**
	 *  Changes the controlled player
	 *
	 *  Changes the players state to controlled or watching depending on the boolean
	 *  overriden by child actor objects
	 *  @param truth if the player should become the controlled player
	 *  @see Command()
	 */
	virtual void ChangePlayer(bool truth) {};
	
	/**
	 * Commands actor to attack.
	 *
	 * Tells the actor to attack. All actors create bullets, in their attack
	 * overriden by child actor objects
	 * @param dt delta time: time elapsed since the previous update.
	 * @see Command()
	 */
	virtual void Attack(float dt) {};

	/**
	 * Commands actor to die
	 *
	 * Tells the actor to die. This puts the actor in an appropriate DEAD state
	 * overriden by child actor objects
	 * @param dt delta time: time elapsed since the previous update.
	 * @see Command()
	 */
	virtual void Die(float dt) {};

	/**
	 * Commands zombie to Chase target player
	 *
	 * Tells the actor to chase a target player
	 * @param player reference to the player
	 */
	virtual void Chase(const Player& player) {};
	
	/** 
	 * Commands zombie to Navigate to target location
	 *
	 * Tells the actor to chase a target player
	 * overriden by child actor objects
	 * @param target 2d position of the target
	 */
	virtual void NavigateTo(const Vec2f& target) {};

	// Setters/Getters

	/**
	 * Gets the direction actor is looking in
	 * @return the look direction as a normalized 2d vector
	 */
	Vec2f GetDirectionVector() const
	{
		Vec2f ret(cosf(GetDirection()), sinf(GetDirection()));
		return ret.GetNormalized();
	}

	/// returns actors movement speed as float
	float Getspeed()				const { return m_speed; }
	/// returns actors x-position as float
	float GetX()					const { return m_x; }
	/// returns actors y-position as float
	float GetY()					const { return m_y; }
	/// returns the looking direction of the actor as float
	float GetDirection()			const { return m_dir; }
	/// returns actors position as vector
	Vec2f GetPosition()				const { return Vec2f( m_x, m_y); }
	/// returns radius of the actor as float
	float GetRadius()				const { return m_radius; }
	/// returns actors tag as const reference
	const ActorTag& GetTag()		const { return m_tag; }
	/// returns true if actor is destroyed and should be deleted
	bool GetDestroyed()				const { return m_destroyed; }
	/// returns true if actor was hit by bullet 
	bool GetIsHit()					const { return m_hit; }
	/// returns actors color as const reference
	const olc::Pixel& GetColor()	const { return m_color; }
	
	/// Sets actors speed
	void SetSpeed(float s)			{ m_speed = s; }
	/// Sets x-position
	void SetX(float x)				{ m_x = x; }
	/// Sets the y-position
	void SetY(float y)				{ m_y = y; }
	/// Sets the looking direction of the actor as radians clamped to 0-2*pi
	void SetDirection(float dir)		
	{ 
		// Make sure, that direction doesn't go over 2PI 
		m_dir = dir;
		if (m_dir > PI2)
			m_dir -= PI2;
		else if (m_dir < 0.0f)
			m_dir += PI2;
	}
	/// Sets radius of the actor
	void SetRadius(float r)				{ m_radius = r; }
	/// Sets the position vector of the actor (input as vector)
	void SetPosition(const Vec2f& pos)  { m_x = pos.x, m_y = pos.y; }
	/// Sets the position vector of the actor (input as floats x, y)
	void SetPosition(float x, float y)  { m_x = x, m_y = y; }
	/// Sets the tag of the actor
	void SetTag(ActorTag tag)			{ m_tag = tag; }
	/// Sets the destroyed boolean of the actor
	void SetDestroyed(bool truth)		{ m_destroyed = truth; }
	/// Sets if the actor is hit
	void SetIsHit(bool truth)			{ m_hit = truth; }
	/// Sets the color of the actor
	void SetColor(olc::Pixel color)		{ m_color = color; }

	static unsigned int m_objectCount;
	unsigned int m_id;

private:
	bool m_hit;
	bool m_destroyed;
	float m_speed;
	float m_x;
	float m_y;
	float m_dir;
	float m_radius;
	ActorTag m_tag;
	olc::Pixel m_color;
};
