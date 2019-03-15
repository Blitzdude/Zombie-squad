#pragma once

#include <math.h>
#include <memory>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

// returns the value squared
template <typename T>
inline auto square(const T& x)
{
	return x * x;
}

template <typename T>
inline auto toRadians(const T& d)
{
	return d * PI / 180;
}

