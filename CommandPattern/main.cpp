#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Command.h"
#include "InputHandler.h"
// #include "Actor.h"
#include "Player.h"



class MainGame : public olc::PixelGameEngine
{
public:
    MainGame();
    ~MainGame(){};

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    

private:
    InputHandler m_inputHandler;
    Player m_player;
};

MainGame::MainGame()
    : m_inputHandler(*this)
{
    sAppName = "Command pattern";
}

bool MainGame::OnUserCreate()
{
    return true;
}

bool MainGame::OnUserUpdate(float fElapsedTime)
{
    // Clear the screen
    Clear(olc::BLACK);

    FillRect(m_player.getXPos(), m_player.getYPos(), 100, 100, olc::BLUE);
    
    m_inputHandler.handleInput();
    Command* command = m_inputHandler.handleInput();
    if (command)
    {
        command->execute(m_player, fElapsedTime);
    }

    return true;
}


int main(int argc, char** argv)
{
    MainGame app;
    app.Construct(800, 600, 1, 1);
    app.Start();

    return 0;
}