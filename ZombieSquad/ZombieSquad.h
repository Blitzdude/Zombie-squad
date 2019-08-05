#pragma once
#include "olcPixelGameEngine.h"
#include "PlayerHandler.h"
#include "ZombieHandler.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Command.h"
#include "Level.h"
#include "Physics.h"

#include <vector>
#include <iostream>

class ZombieSquad : public olc::PixelGameEngine
{


public:
	ZombieSquad();
	
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	void DoInput(float fElapsedTime);
	void DoUpdate(float fElapsedTime);
	void DoDraw();

	void SpawnZombie(int x, int y, float offset = 0.0f);
	
	void SpawnBullet(const Vec2f& pos, float dir, float lifetime);

	

private:
	PlayerHandler m_playerHandler;
	ZombieHandler m_zombieHandler;
	std::vector<Actor*> vecActors;
	Level* m_currentLevel;
	Physics m_physics;

	bool m_isRunning = true;
};

