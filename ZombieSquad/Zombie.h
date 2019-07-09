#pragma once
#include "Actor.h"
class Zombie : public Actor
{
public:
	Zombie();
	Zombie(float x, float y);
	~Zombie();

	virtual void Draw(olc::PixelGameEngine& game) override;
private:
};

