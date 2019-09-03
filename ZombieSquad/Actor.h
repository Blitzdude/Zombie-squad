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

class Actor
{
public:
	Actor();
	virtual ~Actor();

	virtual void Draw(olc::PixelGameEngine& game) = 0;
	virtual void Update(float fElapsedTime) = 0;
	

	// Player commands
	virtual void MoveForward(float dt){};
	virtual void MoveBack(float dt)	{};
	virtual void TurnRight(float dt)  {};
	virtual void TurnLeft(float dt)   {};
	virtual void ChangePlayer(bool truth) {};
	virtual void Attack(float dt) {};
	virtual void Die(float dt) {};

	// Zombie commands
	virtual void Chase(const Player& player) {};
	
	Vec2f GetDirectionVector() const;

	// Setters/Getters
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
