#pragma once

#include "mathUtil.h"

template <class T>
class Vec2 {
public: // methods
	Vec2() {}
	Vec2(T _x, T _y)
		:
		x(_x),
		y(_y)
	{}

	// copy constructor
	Vec2(const Vec2 &vect)
		:
		Vec2(vect.x, vect.y)
	{}

	/* METHODS *///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	// return length squared
	T sqLength() const
	{
		return square(*this);
	}

	// return the vector length
	T length() const
	{
		return sqrt(sqLength());
	}

	// Normalize the vector
	Vec2& Normalize()
	{
		const T len = this->length();
		x /= len;
		y /= len;
		return *this;
	}

	// get the normalized vector
	Vec2 GetNormalized() const
	{
		Vec2 norm = *this;
		norm.Normalize();
		return norm;
	}

	// returns the scalar of 2d cross product
	// it's just the determinant of [x,y] and [x',y']
	static T CrossProduct(const Vec2<T> &v1, const Vec2<T> &v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	// returns the dot product between 2 vectors
	static T DotProduct(const Vec2<T> &v1, const Vec2<T> &v2)
	{
		return (v1.x * v2.x) + (v1.y + v2.y);
	}

	// returns the angle between 2d vectors
	static T AngleBetween(const Vec2<T> &v1, const Vec2<T> &v2)
	{
		T ret = DotProduct(v1, v2) / (v1.length * v2.length);

		return acos(ret);
	}


	/****************************************************************/


	/* OPERATORS *///////////////////////////////////////////////////////
				   //////////////////////////////////////////////////////

	// - operator negate vector
	Vec2 operator-() const
	{
		return Vec2(-x, -y);
	}



	// = operator
	Vec2& operator=(const Vec2 &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	// += operator
	Vec2& operator+=(const Vec2 &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	// + operator
	Vec2 operator+(const Vec2 &other) const
	{
		return Vec2(*this) += other;
	}

	// -= operator
	Vec2& operator-=(const Vec2 &other) 
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// - operator vec2 - vec2
	Vec2 operator-(const Vec2 &other) const
	{
		
		return Vec2(*this) -= other;
	}

	// * operator scalar * vector
	T operator*(const Vec2 &other) const
	{
		return x * other.x + y * other.y;
	}

	// *= operator vector *= scalar
	Vec2& operator*=(const T &other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	// * operator vector * scalar
	Vec2 operator*(const T other) const
	{
		return Vec2(*this) *= other;
	}

	// /= operator vector /= scalar
	Vec2& operator/=(const T &other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	Vec2 operator/(const T &other)
	{
		return Vec2(*this) /= other;
	}

	bool operator==(const Vec2 &other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vec2 &other) const
	{
		return !(*this == other);
	}

	
public: // attributes
	T x;
	T y;
};

