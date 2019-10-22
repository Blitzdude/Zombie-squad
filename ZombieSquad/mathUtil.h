#pragma once

#include <math.h>
#include <memory>


constexpr float PI = 3.14159265f;
constexpr float PI2 = 3.14159265f * 2.0f;

constexpr double PI_D = 3.1415926535897932;
constexpr double PI2_D = 3.1415926535897932 * 2.0f;


/**
* @param x Template input
* @returns input squared 
*/
template <typename T>
inline auto square(const T& x)
{
	return x * x;
}

/**
* Converts an angle degrees to radians
*
* @param d angle as degrees
* @returns angle as radians
*/
template <typename T>
inline auto Deg2Radians(const T& d) 
{
	return d * PI / 180;
}

/**
* Converts an angle radians to degrees
*
* @param d angle as radians
* @returns angle as degrees
*/
template <typename T>
inline auto Rad2Degrees(const T& d)
{
	return d * 180 / PI;
}

/**
* Clamps a value between min and max
*
* Clamps a given value between a range dictated by min and max
*
* @param min,max minimum and maximum values to clamp between
* @param value value to clamp
* @returns min or max if outside the range. Otherwise the value itself
*/
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

/**
* Calculate the manhattan distance from point 1 to point 2
*
* Calculates the distance from point 1 to point 2 using Manhattan distance calculation.
* Distance is the absolute values between x1 - x2 and y1 - y2 added together
* This gives a distance following only x and y axis 
*
* @param x1,y1 x and y coordinates of the point 1
* @param x2,y2 x and y coordinates of point 2
* @returns block distance of these 
*/
template <typename T>
inline auto ManhattanDistance(const T& x1, const T& y1, const T& x2, const T& y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

