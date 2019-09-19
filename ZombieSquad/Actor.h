#pragma once

#include "olcPixelGameEngine.h"
#include "Vec2.h"

/***
Actor is an abstract base class, that holds all common functionality for
Actors within the game.
*/

class Player;

enum class ActorTag {
	NONE = 0,
	PLAYER,
	ZOMBIE,
	BULLET
};


/// Abstract base class for all actors
///
/// Actor acts as a parent class for Player, Zombie and Bullet classes. 
/// As this class is abstract, no objects can or should be instanciated 
/// from this class
class Actor
{
public:
	Actor();
	virtual ~Actor();

	/// Draws the actor
	///
	/// Abstract method used to draw the actor. Must be implemented by children
	/// 
	/// @param game reference to 
	/// @returns no return value
	virtual void Draw(olc::PixelGameEngine& game) = 0;

	/// Updates the actor
	///
	/// Abstract method used to update the actor. Must be implemented by children
	///
	/// @param dt delta time: time elapsed since previous update
	/// @returns no return value
	/// @see DoUpdate()
	virtual void Update(float dt) = 0;
	

	/// Moves a player actor forward
	///
	/// Command moves player actor in the direction its currently facing
	/// overriden by child actor objects
	/// @param dt delta time: time elapsed since the previous update.
	/// @returns no return value
	/// @see Command()
	virtual void MoveForward(float dt) {};

	/// Moves a player actor backwards
	///
	/// Command moves player actor directly away from the direction its currently facing
	/// overriden by child actor objects
	/// @param dt delta time: time elapsed since the previous update.
	/// @returns no return values
	/// @see Command()
	virtual void MoveBack(float dt)	{};
	
	/// Turns player actor right
	///
	/// Command turns player actor clockwise to the right
	/// overriden by child actor objects
	/// @param dt delta time: time elapsed since the previous update.
	/// @returns no return values
	/// @see Command()
	virtual void TurnRight(float dt)  {};

	/// Turns player actor left
	///
	/// Command turns player actor counter-clockwise to the left
	/// overriden by child actor objects
	/// @param dt delta time: time elapsed since the previous update.
	/// @returns no return values
	/// @see Command()
	virtual void TurnLeft(float dt)   {};

	/// Changes the controlled player
	///
	/// Changes the players state to controlled or watching depending on the boolean
	/// overriden by child actor objects
	/// @param truth if the player should become the controlled player
	/// @returns no return values
	/// @see Command()
	virtual void ChangePlayer(bool truth) {};
	
	/// Commands actor to attack.
	///
	/// Tells the actor to attack. All actors create bullets, in their attack
	/// overriden by child actor objects
	/// @param dt delta time: time elapsed since the previous update.
	/// @returns no return values
	/// @see Command()
	virtual void Attack(float dt) {};

	/// Commands actor to die
	///
	/// Tells the actor to die. This puts the actor in an appropriate DEAD state
	/// overriden by child actor objects
	/// @param dt delta time: time elapsed since the previous update.
	/// @returns no return values
	/// @see Command()
	virtual void Die(float dt) {};

	// Zombie commands
	/// Command actor to Chase target player
	///
	/// Tells the actor to chase a target player
	/// overriden by child actor objects
	/// @param player reference to the player
	/// @return no return values
	virtual void Chase(const Player& player) {};
	
	/// Command zombie to Navigate to target location
	///
	/// Tells the actor to chase a target player
	/// overriden by child actor objects
	/// @param target 
	/// @return no return values
	virtual void NavigateTo(const Vec2f& target) {};



	// Setters/Getters
	Vec2f GetDirectionVector() const
	{
		Vec2f ret(cosf(GetDirection()), sinf(GetDirection()));
		return ret.GetNormalized();
	}
	float GetSpeed()			const { return m_speed; }
	float GetX()				const { return m_x; }
	float GetY()				const { return m_y; }
	float GetDirection()		const { return m_dir; }
	Vec2f GetPosition()			const { return Vec2f( m_x, m_y); }
	float GetRadius()			const { return m_radius; }
	const ActorTag& GetTag()	const { return m_tag; }
	bool GetDestroyed()			const { return m_destroyed; }
	bool GetIsHit()				const { return m_hit; }
	
	void SetSpeed(float s)				{ m_speed = s; }
	void SetX(float x)					{ m_x = x; }
	void SetY(float y)					{ m_y = y; }
	void SetDirection(float dir)		
	{ 
		// Make sure, that direction doens't go over 
		m_dir = dir;
		if (m_dir > PI2)
			m_dir -= PI2;
		else if (m_dir < 0.0f)
			m_dir += PI2;
	}
	void SetRadius(float r)				{ m_radius = r; }
	void SetPosition(const Vec2f& pos)  { m_x = pos.x, m_y = pos.y; }
	void SetPosition(float x, float y)  { m_x = x, m_y = y; }
	void SetTag(ActorTag tag)			{ m_tag = tag; }
	void SetDestroyed(bool truth)		{ m_destroyed = truth; }
	void SetIsHit(bool truth)			{ m_hit = truth; }

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
};
