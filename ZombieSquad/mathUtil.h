#pragma once

#include <math.h>
#include <memory>

constexpr float PI = 3.14159265f;
constexpr float PI2 = 3.14159265f * 2.0f;

constexpr double PI_D = 3.1415926535897932;
constexpr double PI2_D = 3.1415926535897932 * 2.0f;


// returns the value squared
template <typename T>
inline auto square(const T& x)
{
	return x * x;
}

template <typename T>
inline auto Deg2Radians(const T& d) 
{
	return d * PI / 180;
}

template <typename T>
inline auto Rad2Degrees(const T& d)
{
	return d * 180 / PI;
}

template <typename T>
inline auto Clamp(const T& min, const T& max, const T& value)
{
	if (value <= min)
	{
		return min;
	}
	else if (value >= max)
	{
		return max;
	}
	else
	{
		return value;
	}
	
}

template <typename T>
inline auto ManhattanDistance(const T& x1, const T& y1, const T& x2, const T& y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

