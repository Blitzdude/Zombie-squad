#pragma once

#include "olcPixelGameEngine.h"
#include "Enemy.h"
#include "Player.h"
#include "AIHandler.h"
#include "InputHandler.h"


class MainGame : public olc::PixelGameEngine
{
public:
    MainGame();
    ~MainGame() {};

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void Input(float deltaTime);
    void Update(float deltaTime);
    void Draw();

private:
    InputHandler m_inputHandler;
    AIHandler m_aiHandler;
    Player m_player;
    Enemy  m_enemy;
    olc::Sprite* sprPlayer;
    olc::Sprite* sprEnemy;
};


