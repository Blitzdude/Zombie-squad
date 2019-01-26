#include "InputHandler.h"
#include "Command.h"

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

Command* InputHandler::handleInput()
{
    if (m_engine->GetKey(olc::W).bHeld) return buttonW;
    if (m_engine->GetKey(olc::A).bHeld) return buttonA;
    if (m_engine->GetKey(olc::S).bHeld) return buttonS;    
    if (m_engine->GetKey(olc::D).bHeld) return buttonD;
    if (m_engine->GetKey(olc::SPACE).bHeld) return buttonSpace;
  

    return nullptr;
}

// binds all buttns to Command class object pointers.
void InputHandler::bindButtons()
{
    buttonW = new MoveUpCommand();
    buttonA = new MoveLeftCommand();
    buttonS = new MoveDownCommand();
    buttonD = new MoveRightCommand();
    buttonSpace = new JumpCommand();
}
