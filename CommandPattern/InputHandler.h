#pragma once
#include "olcPixelGameEngine.h"


class Command;

class InputHandler
{
public:
    InputHandler(olc::PixelGameEngine& engine);
    ~InputHandler();

    Command* handleInput();

    void bindButtons();
private:
    Command* buttonW;
    Command* buttonA;
    Command* buttonS;
    Command* buttonD;
    Command* buttonSpace;

    olc::PixelGameEngine* m_engine;
};

