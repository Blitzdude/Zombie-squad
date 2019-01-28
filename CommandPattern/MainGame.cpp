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

    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            m_vecEnemies.emplace_back(new Enemy(m_player, 400.0f + 50.0f * x, 100.0f + 50.0f * y));

    /*
    m_vecEnemies.emplace_back(new Enemy(m_player, 500.0f, 200.0f));
    m_vecEnemies.emplace_back(new Enemy(m_player, 400.0f, 300.0f));
    */

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

    for (auto enemy : m_vecEnemies)
    {
        Command* enemyCommand = m_aiHandler.handleInput(*enemy);
        if (enemyCommand)
            enemyCommand->execute(*enemy, deltaTime);
    }
}

void MainGame::Update(float deltaTime)
{
    for (auto enemy : m_vecEnemies)
    {
        enemy->update(deltaTime);
    }
}

void MainGame::Draw()
{
    // Clear the screen
    Clear(olc::BLACK);

    SetPixelMode(olc::Pixel::ALPHA);

    #pragma warning (disable : 4244) // warning removed: narrowing conversion from float to int

    DrawSprite(m_player.getXPos(), m_player.getYPos(), sprPlayer, 1);
    DrawCircle(m_player.getXPos(), m_player.getYPos(), 5.0f, olc::MAGENTA);

    // Draw all enemies
    for (auto enemy : m_vecEnemies)
    {
        DrawSprite(enemy->getXPos(), enemy->getYPos(), sprEnemy, 1);
    }

    #pragma warning (default : 4244)

    SetPixelMode(olc::Pixel::NORMAL);
}
