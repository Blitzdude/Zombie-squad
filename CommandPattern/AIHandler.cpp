#include "AIHandler.h"
#include "Actor.h"
#include "Command.h"

AIHandler::AIHandler()
{
    bindCommands();
}

AIHandler::~AIHandler()
{
    // Cleaning, because we are nice
    delete UpCommand;    UpCommand = nullptr;
    delete DownCommand;  DownCommand = nullptr;
    delete LeftCommand;  LeftCommand = nullptr;
    delete RightCommand; RightCommand = nullptr;

}

Command* AIHandler::handleInput(const Actor &actor)
{
    if (0) return UpCommand;
    if (0) return DownCommand;
    if (0) return LeftCommand;
    if (1) return RightCommand;

    return nullptr;
}

// binds all buttns to Command class object pointers.
void AIHandler::bindCommands()
{
    UpCommand = new MoveUpCommand();
    DownCommand = new MoveLeftCommand();
    LeftCommand = new MoveDownCommand();
    RightCommand = new MoveRightCommand();
}
