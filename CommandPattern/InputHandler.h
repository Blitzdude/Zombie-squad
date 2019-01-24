#pragma once
#include "Command.h"
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

InputHandler::InputHandler(olc::PixelGameEngine& engine)
    : m_engine(&engine)
{
}

InputHandler::~InputHandler()
{
}

inline Command* InputHandler::handleInput()
{
    
    return nullptr;
}

inline void InputHandler::bindButtons()
{
    buttonW     = new MoveUpCommand();
    buttonA     = new MoveLeftCommand();
    buttonS     = new MoveDownCommand();
    buttonD     = new MoveRightCommand();
    buttonSpace = new JumpCommand();
}
