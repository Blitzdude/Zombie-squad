#pragma once
#include "Actor.h"
#include "StructCollection.h"

class Physics
{
public:

	float static isColliding(const Actor &act, const Edge &edge);
	float static isColliding(const Actor& lhs, const Actor& rhs);
	void static resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove);
	void static resolveCircleCircle(Actor* lhs, Actor* rhs, float overlap);
	bool static CheckLineIntersection(Ray& r, Edge& e, IntersectResult* point = nullptr);

private:
	Physics() {};
	~Physics() {};


};