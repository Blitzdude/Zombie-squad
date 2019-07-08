#pragma once
#include "Actor.h"
#include "StructCollection.h"

class Physics
{
public:
	Physics();
	~Physics();

	float isColliding(const Actor &pos, const Edge &edge);
	void resolveEdgeCircle(Actor* act, Vec2f normal, float distToMove);



};