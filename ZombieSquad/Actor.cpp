#include "Actor.h"

unsigned int Actor::m_objectCount = 0;

// TODO: Base variables should propably be initilized in Actor
Actor::Actor()
{
	m_id = m_objectCount++;
	std::cout << "Object count is: " << m_objectCount << "\n";
}

Actor::~Actor()
{
	m_objectCount--;
	std::cout << "Object count is: " << m_objectCount << "\n";
}

Vec2f Actor::GetDirectionVector()
{
	Vec2f ret(cosf(GetDirection()), sinf(GetDirection()));
	return ret.GetNormalized();
}
