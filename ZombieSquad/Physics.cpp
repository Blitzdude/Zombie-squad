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

float Physics::isColliding(const Actor& lhs, const Actor& rhs)
{
	// Vector from lhs to rhs
	Vec2f vecLR(rhs.GetPosition() - lhs.GetPosition());
	float length = vecLR.Length();
	// if length of this vector is less then radius of both -> circles are collidign
	if (rhs.GetRadius() + lhs.GetRadius() >= length)
	{
		// if they are, calculate absolute overlap and return it
		float ret = fabs(vecLR.Length() - rhs.GetRadius() - lhs.GetRadius());
		return ret;
	}
	// otherwise return 0 for no overlap
	return 0.0f;
}


void Physics::resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove)
{
	// move the actor in the direction of the normal
	act->SetPosition(act->GetPosition() + normal * distToMove);
	// calculate the movement vector
	// add the movement vector to the position of the Actor

}

void Physics::resolveCircleCircle(Actor* lhs, Actor* rhs, float overlap)
{
	// determine the vector from lhs to rhs
	Vec2f direction(rhs->GetPosition() - lhs->GetPosition());
	// normalize it
	direction.Normalize();
	// move lhs in negative direction half way and rhs the other way
	lhs->SetPosition(lhs->GetPosition() - direction * overlap);
	rhs->SetPosition(rhs->GetPosition() + direction * overlap);

}
