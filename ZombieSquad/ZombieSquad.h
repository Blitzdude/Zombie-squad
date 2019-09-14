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
	bool CheckVictory();

	Player* SpawnPlayer(float xPos, float yPos, float dir, int playerNum, ZombieSquad& game, PlayerHandler& playerHandler, float offset = 0.0f, bool startingPlayer = false);
	void SpawnZombie(int x, int y, float offset = 0.0f);
	void SpawnBullet(const Vec2f& pos, float dir, float lifetime, float speed, ActorTag tag);

	void AddActors(); 

	// Getters
	const std::vector<Actor*>& GetActors() { return vecActors; };

	ZombieHandler m_zombieHandler;
	PlayerHandler m_playerHandler;

	Level* m_currentLevel;
private:

	std::vector<Actor*> vecActors;
	std::vector<Actor*> vecActorsToAdd;

	/// For testing purpouses only.
	std::vector<std::pair<int, int>> vecDebugPath;

	bool m_isWin;
	bool m_isGameOver;
	bool m_isRunning;
};

