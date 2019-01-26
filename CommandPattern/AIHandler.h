#pragma once

class Actor;
class Command;

class AIHandler
{
public:
    AIHandler();
    ~AIHandler();

    Command* handleInput(const Actor& actor);
    void bindCommands();

private:
    Command* UpCommand;
    Command* DownCommand;
    Command* LeftCommand;
    Command* RightCommand;
};

