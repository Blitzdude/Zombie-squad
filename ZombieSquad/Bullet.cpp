#include "Bullet.h"

void Bullet::Draw(olc::PixelGameEngine& game)
{
	game.FillCircle(GetX(), GetY(), GetRadius());
}
