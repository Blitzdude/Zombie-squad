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
	// Check that line formed by velocity vector, intersects with line segment
	Vec2f vecSE(edge.end - edge.start); // edge vector
	Vec2f vecPS(act.GetPosition() - edge.start); // vector from actor to edge start
	float edgeLength = vecSE.SqLength(); // edge length squared

	// Calculate the closest (i.e perpendicular) point on the line by projecting
	// vec_PS to vec_SE and clamping its value between zero and it's length
	// Then normalize the value by dividing the value with edge length
	float t = ZClamp(0.0f, edgeLength, Vec2f::DotProduct(vecSE, vecPS)) / edgeLength;
	Vec2f vecClosestPoint = edge.start + vecSE * t;

	Vec2f vecPC(act.GetPosition() - vecClosestPoint); // Actor position to closest position
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


void Physics::resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove)
{
	// move the actor in the direction of the normal
	act->SetPosition(act->GetPosition() + normal * distToMove);
	// calculate the movement vector
	// add the movement vector to the position of the Actor

}
