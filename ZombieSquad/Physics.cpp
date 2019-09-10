#include "Physics.h"
#include "Bullet.h"
#include "mathUtil.h"
#include <assert.h>


// Function returns the amount of overlap for circle and edge. if they are not colliding, function returns 0.0f
float Physics::isColliding(const Actor& act, const Edge& edge)
{
	// Check that line formed by velocity vector, intersects with line segment
	Vec2f vecSE(edge.end - edge.start); // edge vector
	Vec2f vecPS(act.GetPosition() - edge.start); // vector from actor to edge start
	float edgeLength = vecSE.SqLength(); // edge length squared

	// Calculate the closest (i.e perpendicular) point on the line by projecting
	// vec_PS to vec_SE and clamping its value between zero and it's length
	// Then normalize the value by dividing the value with edge length
	float t = Clamp(0.0f, edgeLength, Vec2f::DotProduct(vecSE, vecPS)) / edgeLength;
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
	// If actor is bullet, destroy bullet.
	if (act->GetTag() == ActorTag::BULLET)
	{
		act->SetIsHit(true);
	}
	else {
		// move the actor in the direction of the normal
		act->SetPosition(act->GetPosition() + normal * distToMove);
		// calculate the movement vector
		// add the movement vector to the position of the Actor
	}

}

void Physics::resolveCircleCircle(Actor* lhs, Actor* rhs, float overlap)
{

	if (lhs->GetTag() == ActorTag::BULLET)
	{
		if (static_cast<Bullet*>(lhs)->GetOwner() != rhs->GetTag())
		{
			// if Bullets owner is different then other, kill them
 			rhs->SetIsHit(true);
			lhs->SetIsHit(true);
		}
	}
	else if (rhs->GetTag() == ActorTag::BULLET)
	{
		if (static_cast<Bullet*>(rhs)->GetOwner() != lhs->GetTag())
		{
			// if Bullets owner is different then other, kill them
  			rhs->SetIsHit(true);
			lhs->SetIsHit(true);
		}
	}
	else {
		// Actors are zombies or players. 

		// determine the vector from lhs to rhs
		Vec2f direction(rhs->GetPosition() - lhs->GetPosition());
		// normalize it
		direction.Normalize();
		// move lhs in negative direction half way and rhs the other way
		lhs->SetPosition(lhs->GetPosition() - direction * (overlap / 2.0f));
		rhs->SetPosition(rhs->GetPosition() + direction * (overlap / 2.0f));
	}
}

bool Physics::CheckLineIntersection(Ray& r, Edge& e, IntersectResult* point)
{
	// Source: http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
	// calculate t1 and t2 where t1 is 

	// calculate denominator 
	float denominator = (e.end.x - e.start.x) * (r.start.y - r.end.y) - (r.start.x - r.end.x) * (e.end.y - e.start.y);

	// check for division by zero error
	if (denominator != 0.0f)
	{
		// calculate t1 and t2 values
		float t1 = ((e.start.y - e.end.y) * (r.start.x - e.start.x) + (e.end.x - e.start.x) * (r.start.y - e.start.y))
			/ denominator;

		float t2 = ((r.start.y - r.end.y) * (r.start.x - e.start.x) + (r.end.x - r.start.x) * (r.start.y - e.start.y))
			/ denominator;

		if ((t1 >= 0.0f && t1 <= 1.0f) && (t2 >= 0.0f && t2 <= 1.0f)) // line segments are intersecting if true
		{
			if (point != nullptr)
			{
				// calculate intersection point
				point->px = r.start.x + t1 * (r.end.x - r.start.x);
				point->py = r.start.y + t1 * (r.end.y - r.start.y);
				point->t = t1;
			}
			return true;
		}
	}

	return false; // no intersection was found
}
