#pragma once

#include "Vec2.h"
#include "Actor.h"

/**
* Holds all data relating to edges
*/
struct Edge
{
	Edge() {}
	Edge(const Edge& other)
		: start(other.start), end(other.end), normal(other.normal)
	{}
	Vec2f start;
	Vec2f end;
	Vec2f normal;
};

/**
* Rays are lines with start and end. 
*/
struct Ray
{
	Ray() {}
	Ray(Vec2f s, Vec2f e) 
		: start(s), end(e)
	{}
	Ray(const Ray& other)
		: start(other.start), end(other.end)
	{}

	Vec2f start;
	Vec2f end;
};

/**
* Holds the results between intersections
*/
struct IntersectResult
{
	IntersectResult()
	{
		px = 0.0f;
		py = 0.0f;
		t = -1.0f;
	};

	float px, py;
	float t;
};

/**
* Holds results of collision between Circle and Edge
*/
struct CECollision 
{
	CECollision(Actor* act, const Vec2f& norm, float dist)
		: actor(act), normal(norm), distance(dist)
	{}
	Actor* actor; // Actor to move
	Vec2f normal; // direction in which to move
	float distance; // distance to move
};


/**
* Holds results of collision between Circle and Edge
*/
struct CCCollision
{
	CCCollision(Actor* l, Actor* r, float o)
		: lhs(l), rhs(r), overlap(o)
	{}
	Actor* lhs; // Actor to move
	Actor* rhs; // direction in which to move
	float overlap; // distance to move
};