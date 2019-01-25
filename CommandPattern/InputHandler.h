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
    bindButtons();
}

InputHandler::~InputHandler()
{
    // Cleaning, because we are nice
    delete buttonW; buttonW = nullptr;
    delete buttonA; buttonA = nullptr;
    delete buttonS; buttonS = nullptr;
    delete buttonD; buttonD = nullptr;
    delete buttonSpace; buttonSpace = nullptr;

}

inline Command* InputHandler::handleInput()
{
    if (m_engine->GetKey(olc::W).bPressed)
    {
        return buttonW;
    }
    if (m_engine->GetKey(olc::A).bPressed)
    {
        return buttonA;
    }
    if (m_engine->GetKey(olc::S).bPressed)
    {
        return buttonS;
    }
    if (m_engine->GetKey(olc::D).bPressed)
    {
        return buttonD;
    }
    if (m_engine->GetKey(olc::SPACE).bPressed)
    {
        return buttonSpace;
    }

    return nullptr;
}

// binds all buttns to Command class object pointers.
inline void InputHandler::bindButtons()
{
    buttonW     = new MoveUpCommand();
    buttonA     = new MoveLeftCommand();
    buttonS     = new MoveDownCommand();
    buttonD     = new MoveRightCommand();
    buttonSpace = new JumpCommand();
}
