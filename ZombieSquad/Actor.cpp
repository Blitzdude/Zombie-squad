#include "Actor.h"

unsigned int Actor::m_objectCount = 0;

// TODO: Base variables should propably be initilized in Actor
Actor::Actor()
{
	m_destroyed  = false;
	m_dir		 = 0.0f;
	m_radius	 = 0.0f;
	m_speed		 = 0.0f;
	m_tag		 = ActorTag::NONE;
	m_x			 = 0.0f;
	m_y			 = 0.0f;

	m_id = m_objectCount++;
	std::cout << "Object count is: " << m_objectCount << "\n";
}

Actor::~Actor()
{
	m_objectCount--;
	std::cout << "Object count is: " << m_objectCount << "\n";
}

Vec2f Actor::GetDirectionVector() const
{
	Vec2f ret(cosf(GetDirection()), sinf(GetDirection()));
	return ret.GetNormalized();
}
