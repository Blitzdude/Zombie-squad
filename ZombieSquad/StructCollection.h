#pragma once

#include "Vec2.h"
#include "Actor.h"

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

struct CECollision 
{
	CECollision(Actor* act, const Vec2f& norm, float dist)
		: actor(act), normal(norm), distance(dist)
	{}
	Actor* actor; // Actor to move
	Vec2f normal; // direction in which to move
	float distance; // distance to move
};

struct CCCollision
{
	CCCollision(Actor* l, Actor* r, float o)
		: lhs(l), rhs(r), overlap(o)
	{}
	Actor* lhs; // Actor to move
	Actor* rhs; // direction in which to move
	float overlap; // distance to move
};