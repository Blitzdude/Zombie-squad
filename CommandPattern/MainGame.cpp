#include "MainGame.h"
#include "Command.h"

MainGame::MainGame()
    : m_inputHandler(*this)
{
    sAppName = "Command pattern";
}

bool MainGame::OnUserCreate()
{
    sprPlayer = new olc::Sprite("player.png");
    sprEnemy = new olc::Sprite("zombie.png");
    return true;
}

bool MainGame::OnUserUpdate(float fElapsedTime)
{
    // Clear the screen
    Clear(olc::BLACK);

    SetPixelMode(olc::Pixel::ALPHA);
    DrawSprite(m_player.getXPos(), m_player.getYPos(), sprPlayer, 2);
    DrawSprite(m_enemy.getXPos(), m_enemy.getYPos(),sprEnemy, 2); 
    SetPixelMode(olc::Pixel::NORMAL);

    Command* command = m_inputHandler.handleInput();
    if (command)
        command->execute(m_player, fElapsedTime);
    
    
    Command* enemyCommand = m_aiHandler.handleInput(m_enemy);
    if (enemyCommand)
        enemyCommand->execute(m_enemy, fElapsedTime);

    return true;
}
