#pragma once

/* Actor base class ment for inheritance*/
class Actor
{
public:
    Actor();
    virtual ~Actor();

    virtual void draw() const = 0;

    virtual void jump();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void moveUp();
    virtual void moveDown();
private:
    float m_xPos;
    float m_yPos;
};
