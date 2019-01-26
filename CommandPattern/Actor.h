#pragma once

/* Actor base class meant for inheritance*/
class Actor
{
public:
    Actor() {};
    virtual ~Actor() {};

    virtual void jump(float deltaTime);
    virtual void moveLeft(float deltaTime);
    virtual void moveRight(float deltaTime);
    virtual void moveUp(float deltaTime);
    virtual void moveDown(float deltaTime);

    // Getters/setters
    float getXPos() {return m_xPos;};
    float getYPos() { return m_yPos;};

    void setXPos(float val) { m_xPos = val;};
    void setYPos(float val) { m_yPos = val; };

private:
    float m_xPos;
    float m_yPos;
};
