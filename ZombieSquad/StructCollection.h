#pragma once

#include "Vec2.h"

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