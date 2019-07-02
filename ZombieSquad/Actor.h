#pragma once

#include "olcPixelGameEngine.h"

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

	void SetX(float x)			 { m_x = x; }
	void SetY(float y)			 { m_y = y; }
	void SetDirection(float dir) { m_dir = dir; }


private:
	float m_x;
	float m_y;
	float m_dir;
};
