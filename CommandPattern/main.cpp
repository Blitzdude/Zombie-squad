#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class MainGame : public olc::PixelGameEngine
{
public:
    MainGame();
    ~MainGame(){};

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:

};

MainGame::MainGame()
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

    FillRect(100, 100, 100, 100, olc::BLUE);

    return true;
}


int main(int argc, char** argv)
{
    MainGame app;
    app.Construct(800, 600, 1, 1);
    app.Start();

    return 0;
}