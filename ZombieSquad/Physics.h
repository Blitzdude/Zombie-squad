#pragma once
#include "Actor.h"
#include "StructCollection.h"

/** 
* Holds functions for resolving Physics collisions
* 
* Physics is a completely static class. It holds all necessary functions for
* detecting and resolving different physics collisions. This collisions are: 
*
* <ul>
*	<li>Circle - Circle</li>
*	<li>Circle - Edge</li>
*	<li>Ray - Edge</li>
* </ul>
*/
class Physics
{
public:

	/**
	* Checks collision between Actor and Edge
	*
	* Checks if Actor is overlapping with an edge by checking the Actors 
	* Position with the closest (perpendicular) point to edge. 
	*
	* @param act Reference to the actor
	* @param edge Reference to edge in map
	* @return Amount the actor is overlapping the edge as float. returns 0.0f if no overlap 
	*/
	float static isColliding(const Actor &act, const Edge &edge);
	
	/**
	* Checks collision between Actor and another Actor
	*
	* Checks if Actor is overlaping another Actor by calculating
	* the distance between the two actors and comparing it to their radiuses.
	*
	* @param lhs Reference to the left hand side actor
	* @param rhs Reference to the right hand side actor
	* @return Amount the actors are overlapping as absolute value. returns 0.0f if no overlap.
	*/
	float static isColliding(const Actor& lhs, const Actor& rhs);

	/**
	* Resolves a know collision between Actor and Edge
	*
	* Resolves a know collision between an actor and edge. Bullets are destroyed and
	* other actors are moved in the direction of the edges normal by the amount of overlap.
	*
	* @param act Reference to the actor
	* @param normal Normalized direction vector non-Bullet actors are moved towards.
	* @param distToMove The distance to move a non-Bullet actor along the normal.
	*/
	void static resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove);

	/**
	* Resolves a know collision between two Actors
	*
	* Resolves a know collision between an two Actor objects. If either actor is a Bullet class, 
	* the bullet is destroyed and the other actor takes damage. Otherwise both actors are
	* moved away from each other half the amount of overlap.
	*
	* @param lhs Reference to the left hand side actor
	* @param rhs Reference to the right hand side actor
	* @param overlap Amount of overlap between actors
	*/
	void static resolveCircleCircle(Actor* lhs, Actor* rhs, float overlap);
	
	/**
	* Checks if a Ray and an Edge intersect
	*
	* Calculates if a Ray and an Edge intersect. 
	* The method is described in more detail in the following link:
	* http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
	* 
	* @param r Reference to the left hand side actor
	* @param e Reference to the right hand side actor
	* @param point Amount of overlap between actors
	*/
	bool static CheckLineIntersection(Ray& r, Edge& e, IntersectResult* point = nullptr);

private:
	Physics() {};
	~Physics() {};


};