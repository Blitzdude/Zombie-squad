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
	float fLineX1 = edge.end.x - edge.start.x;
	float fLineY1 = edge.end.y - edge.start.y;

	float fLineX2 = act.GetX() - edge.start.x;
	float fLineY2 = act.GetY() - edge.start.y;

	float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

	// This is nifty - It uses the DP of the line segment vs the line to the object, to work out
	// how much of the segment is in the "shadow" of the object vector. The min and max clamp
	// this to lie between 0 and the line segment length, which is then normalised. We can
	// use this to calculate the closest point on the line segment
	float t = std::max(0.0f, std::min(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;

	// Which we do here
	float fClosestPointX = edge.start.x + t * fLineX1;
	float fClosestPointY = edge.start.y + t * fLineY1;

	Vec2f vecPC(act.GetX() - fClosestPointX, act.GetY() - fClosestPointY); // Actor position to closest position
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
/*
float Physics::isColliding(const Actor& act, const Edge& edge)
{

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
*/

void Physics::resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove)
{
	// move the actor in the direction of the normal
	act->SetPosition(act->GetPosition() + normal * distToMove);
	// calculate the movement vector
	// add the movement vector to the position of the Actor

}
