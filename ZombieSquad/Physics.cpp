#include "Physics.h"
#include "mathUtil.h"
#include <assert.h>


Physics::Physics()
{
}


Physics::~Physics()
{
}

float Physics::isColliding(const Actor& act, const Edge& edge)
{
	// Get the distance to the closest point on the edge
	Vec2f vecPS = (act.GetPosition() - edge.start); // Actor Position to Edge start
	Vec2f vecSE = (edge.end - edge.start);
		// Project the position of the Actor onto the edge
	float value = Vec2f::DotProduct(vecSE, vecPS);
		// clamp the received value between 0.0 and 1.0
	value = ZClamp(0.0f, vecSE.Length(), value);
	value = value / vecSE.Length();
	// calculate the closest point to actor
	Vec2f vecPC = (edge.start + vecSE * value) - vecPS; // Actor position to closest position
	float distance = vecPC.Length(); // PERFORMANCE: This could use squared pythagoran distance for better performance
	// If the distance between value point x is x < actor radius --> objects are colliding
	if (distance <= act.GetRadius())
	{
		// Objects are colliding
		// return the penetration depth
		return act.GetRadius() - distance;
	}
	// Objects are not colliding
	return 0.0f;
}

void Physics::resolveEdgeCircle(Actor& pos, Vec2f normal, float distToMove)
{
	// move the actor in the direction of the normal
		// calculate the movement vector
		// add the movement vector to the position of the Actor

}
