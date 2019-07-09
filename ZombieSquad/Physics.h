#pragma once
#include "Actor.h"
#include "StructCollection.h"

class Physics
{
public:
	Physics();
	~Physics();

	float isColliding(const Actor &act, const Edge &edge);
	float isColliding(const Actor& lhs, const Actor& rhs);
	void resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove);
	void resolveCircleCircle(Actor* lhs, Actor* rhs, float overlap);



};