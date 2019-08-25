#pragma once
#include "olcPixelGameEngine.h"
#include "Zombie.h"
#include "Bullet.h"
#include "ZombieHandler.h"
#include "PlayerHandler.h"
#include "Command.h"
#include "Level.h"

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
	
	void SpawnBullet(const Vec2f& pos, float dir, float lifetime, float speed, ActorTag tag);

	// Getters
	const std::vector<Actor*>& GetActors() { return vecActors; };

	ZombieHandler m_zombieHandler;
	PlayerHandler m_playerHandler;
private:

	std::vector<Actor*> vecActors;
	Level* m_currentLevel;

	bool m_isRunning = true;
};

