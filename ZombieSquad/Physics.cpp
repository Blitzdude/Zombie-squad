#include "Physics.h"
#include "mathUtil.h"


Physics::Physics()
{
}


Physics::~Physics()
{
}

float Physics::isColliding(const Actor& pos, const Edge& edge)
{
	// Get the distance to the closest point on the edge
	Vec2f vecPS = ( edge.start - pos.GetPosition()); // Actor Position to Edge start
	Vec2f vecSE = (edge.end - edge.start);
		// Project the position of the Actor onto the edge
	float value = Vec2f::DotProduct(vecPS, vecSE) / vecSE.Length();
		// clamp the received value between 0.0 and 1.0
	value = Clamp(0.0f, 1.0f, value);
	// If the distance between value point x is x < actor radius --> objects are colliding
	// Determine penetration depth of actor 
	// return the penetration depth
	return 0.0f;
}

void Physics::resolveEdgeCircle(Actor& pos, Vec2f normal, float distToMove)
{
	// move the actor in the direction of the normal
		// calculate the movement vector
		// add the movement vector to the position of the Actor

}
