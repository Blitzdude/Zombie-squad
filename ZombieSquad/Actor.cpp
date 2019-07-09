#include "Actor.h"

unsigned int Actor::m_objectCount = 0;

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
