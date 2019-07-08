#pragma once

#include "olcPixelGameEngine.h"
#include "Vec2.h"

/***
Actor is an abstract base class, that holds all common functionality for
Actors within the game.
*/
class Actor
{
public:
	Actor();
	virtual ~Actor();

	virtual void Draw(olc::PixelGameEngine& game) = 0;

	// Player commands
	virtual void MoveForward(float fElapsedTime){};
	virtual void MoveBack(float fElapsedTime)	{};
	virtual void TurnRight(float fElapsedTime)  {};
	virtual void TurnLeft(float fElapsedTime)   {};

	// Zombie commands

	// Setters/Getters
	float GetX()		 const { return m_x; }
	float GetY()		 const { return m_y; }
	float GetDirection() const { return m_dir; }
	Vec2f GetPosition()  const { return Vec2f( m_x, m_y); }
	float GetRadius()	 const { return m_radius; }

	void SetX(float x)			 { m_x = x; }
	void SetY(float y)			 { m_y = y; }
	void SetDirection(float dir) { m_dir = dir; }
	void SetRadius(float r)		 { m_radius = r; }
	void SetPosition(const Vec2f& pos) { m_x = pos.x, m_y = pos.y; }
	void SetPosition(float x, float y) { m_x = x, m_y = y; }


	bool m_colliding = false;

private:
	float m_x;
	float m_y;
	float m_dir;
	float m_radius;
};
