#pragma once

#include "mathUtil.h"

template <class T>
class Vec2 {
public: 
	Vec2()
		: 
		x(0), 
		y(0)
	{}
	Vec2(T _x, T _y)
		:
		x(_x),
		y(_y)
	{}

	/**  copy constructor */
	Vec2(const Vec2 &vect)
		:
		Vec2(vect.x, vect.y)
	{}

	/* METHODS *///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	/** Returns vectors length squared */
	T SqLength() const
	{
		T ret = (this->x * this->x + this->y * this->y);
		return ret;
	}

	/// return the vectors length
	T Length() const
	{
		T ret = sqrtf(SqLength());
		return ret;
	}

	/** Normalizes the vector */
	Vec2& Normalize()
	{
		const T len = this->Length();
		x /= len;
		y /= len;
		return *this;
	}

	/** 
	* Returns the vector rotated by radians
	* @param rads of radians to rotate
	*/
	Vec2 GetRotated(T rads)
	{
		Vec2<T> ret = *this;
		ret.x = cos(rads) * this->x - sin(rads) * this->y;
		ret.y = sin(rads) * this->x + cos(rads) * this->y;
		return ret;
	}

	/** Returns the normalized vector */
	Vec2 GetNormalized() const
	{
		Vec2 norm = *this;
		norm.Normalize();
		return norm;
	}

	/** 
	* Returns the scalar of 2d cross product
	* it's just the determinant of [x,y] and [x',y']
	*/
	static T CrossProduct(const Vec2<T> &v1, const Vec2<T> &v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	/** returns the dot product between 2 vectors
	*/
	static T DotProduct(const Vec2<T> &v1, const Vec2<T> &v2)
	{
		T ret = (v1.x * v2.x) + (v1.y * v2.y);
		return ret;
	}

	/** returns the angle between 2d vectors
	*/
	static T AngleBetween(const Vec2<T> &v1, const Vec2<T> &v2)
	{
		T dot = DotProduct(v1, v2);
		T det = v1.x*v2.y - v1.y*v2.x;
		T ret = atan2(det, dot);

		return ret;
	}

	/** returns the angle between 2d vectors
	*/
	static T PolarAngle(const Vec2<T> &v1)
	{
		// reference is unit vector along x- axis
		Vec2<T> ref(1.0f, 0.0f);

		T dot = DotProduct(v1, ref);
		T det = v1.x*ref.y - v1.y*ref.x;
		T ret = atan2(det, dot);
		if (ret < 0.0f)
			ret = (PI + ret) + PI; // ret is negative
		return ret;
	}

	/**
	*/
	static bool IsLeft(const Vec2<T> &a, const Vec2<T> &b, const Vec2<T> &point)
	{
		// return CrossProduct(b - a, c - a) > 0;
		return ((b.x - a.x)*(point.y - a.y) - (b.y - a.y)*(point.x - a.x)) > 0;
	}

	/** returns the distance between 2d points
	*/
	static T DistanceBetween(const Vec2<T>& a, const Vec2<T>& b)
	{
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));

	}
	/****************************************************************/


	/* OPERATORS *///////////////////////////////////////////////////////
				   //////////////////////////////////////////////////////

	/** - operator negate vector
	*/
	Vec2 operator-() const
	{
		return Vec2(-x, -y);
	}



	/** = operator
	*/
	Vec2& operator=(const Vec2 &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	/** += operator
	*/
	Vec2& operator+=(const Vec2 &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	/** + operator
	*/
	Vec2 operator+(const Vec2 &other) const
	{
		return Vec2(*this) += other;
	}

	/** -= operator
	*/
	Vec2& operator-=(const Vec2 &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	/** - operator vec2 - vec2
	*/
	Vec2 operator-(const Vec2 &other) const
	{

		return Vec2(*this) -= other;
	}

	/** * operator scalar * vector
	*/
	T operator*(const Vec2 &other) const
	{
		return x * other.x + y * other.y;
	}

	/** *= operator vector *= scalar
	*/
	Vec2& operator*=(const T &other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	/** * operator vector * scalar
	*/
	Vec2 operator*(const T other) const
	{
		return Vec2(*this) *= other;
	}

	/** /= operator vector /= scalar
	*/
	Vec2& operator/=(const T &other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	/** / operator vector / scalar
	*/
	Vec2 operator/(const T &other)
	{
		return Vec2(*this) /= other;
	}

	/**
	* == comparison operator
	*/
	bool operator==(const Vec2 &other) const
	{
		return x == other.x && y == other.y;
	}

	/**
	* != comparison operator
	*/
	bool operator!=(const Vec2 &other) const
	{
		return !(*this == other);
	}


public: // attributes
	T x;
	T y;
};

typedef Vec2<float> Vec2f;
