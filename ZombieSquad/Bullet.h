#pragma once
#include "Actor.h"
#include "State.h"

class Bullet :
	public Actor
{
public:
	Bullet(const Vec2f& pos, float dir, float s, float l, ActorTag owner);
	~Bullet();

	/// Draws the bullet
	///
	/// Draws the bullet actor. 
	/// @param game reference to the engine instance
	/// @returns no return vale
	void Draw(olc::PixelGameEngine& game) override;

	/// Updates the bullet
	///
	/// Draws the bullet actor. 
	/// @param dt delta time
	/// @returns no return vale
	void Update(float dt) override;

	// Getters
	const ActorTag& GetOwner() { return m_owner; };

	float m_lifeTime;
private:

	ActorTag m_owner;
	BulletState* m_currentState;
	float m_radius;
};

