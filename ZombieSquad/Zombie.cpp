#include "Zombie.h"

/*
Zombie::Zombie()
{
	const float ZOMBIE_SIZE = 6.0f;

	SetX(0.0f);
	SetY(0.0f);
	SetRadius(ZOMBIE_SIZE);
	SetDirection(0.0f);
}
*/

Zombie::Zombie(float x, float y)
{
	const float ZOMBIE_SIZE = 6.0f;
	SetX(x);
	SetY(y);
	SetRadius(ZOMBIE_SIZE);
	SetDirection(0.0f);
	SetTag(ActorTag::ZOMBIE);
	std::cout << "Zombie created\n";
}

Zombie::~Zombie()
{
	std::cout << "Zombie destroyed\n";
}


void Zombie::Draw(olc::PixelGameEngine& game)
{

	// TODO: Size should be a variable, static maybe? 
	game.FillCircle((int32_t)GetX(), (int32_t)GetY(), (int32_t)GetRadius(), olc::DARK_GREEN);

	game.DrawLine((int32_t)GetX(), (int32_t)GetY(),
		(int32_t)(GetX() + cosf(GetDirection()) * GetRadius()),
		(int32_t)(GetY() + sinf(GetDirection()) * GetRadius()),
		olc::RED);

}
