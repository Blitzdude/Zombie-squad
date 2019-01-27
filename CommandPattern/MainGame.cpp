#include "MainGame.h"
#include "Command.h"

MainGame::MainGame()
    : m_inputHandler(*this)
    , m_aiHandler(m_player)
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
    Input(fElapsedTime);
    Update(fElapsedTime);
    Draw();

    return true;
}

void MainGame::Input(float deltaTime)
{
    Command* command = m_inputHandler.handleInput();
    if (command)
        command->execute(m_player, deltaTime);

    Command* enemyCommand = m_aiHandler.handleInput(m_enemy);
    if (enemyCommand)
        enemyCommand->execute(m_enemy, deltaTime);
}

void MainGame::Update(float deltaTime)
{
    m_enemy.update(deltaTime);

}

void MainGame::Draw()
{
    // Clear the screen
    Clear(olc::BLACK);

    SetPixelMode(olc::Pixel::ALPHA);

    #pragma warning (disable : 4244) // warning removed: narrowing conversion from float to int

    DrawSprite(m_player.getXPos(), m_player.getYPos(), sprPlayer, 2);
    DrawSprite(m_enemy.getXPos(), m_enemy.getYPos(), sprEnemy, 2);
    #pragma warning (default : 4244)

    SetPixelMode(olc::Pixel::NORMAL);
}
