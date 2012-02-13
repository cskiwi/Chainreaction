#pragma once

#include "Vector2D.h"

// A 2D point class
//
// NOTE: This class is designed for math educational purposes only. 
// The point class of a real game engine would use all kinds of optimization tricks,
// but this would make the code harder to read. However, a good C++ compiler will usually 
// optimize the hell out of the code anyway, so in practice, this design is not as bad as it looks.
//
struct Point2D
{
    // The first coordinate
    Scalar x;

    // The second coordinate
    Scalar y;

    // Default constructor gives the origin point
    Point2D() : x(0), y(0) {}

    // Constructor
    // Usage: Point2D p(1,2);
    Point2D(Scalar x, Scalar y) : x(x), y(y) {}

    // Translation
    // Usage: Point2D q = p + v;
    //   where p is a point and v is a vector
    friend Point2D operator+ (Point2D p, Vector2D v)
    {
        return Point2D(p.x+v.x, p.y+v.y);
    }

    // Translation
    // Usage: Point2D q = v + p;
    //   where p is a point and v is a vector
    friend Point2D operator+ (Vector2D v, Point2D p)
    {
        return Point2D(p.x+v.x, p.y+v.y);
    }

    // Translation
    // Usage: Point2D q = p - v;
    //   where p is a point and v is a vector
    friend Point2D operator- (Point2D p, Vector2D v)
    {
        return Point2D(p.x-v.x, p.y-v.y);
    }

    // Barycentric interpolation between two points
    // Usage: Point2D r = lerp(p,q,0.5)
    //   where p and q are points.
    friend Point2D lerp(Point2D from, Point2D to, Scalar lambda)
    {
        return from + (to-from) * lambda;
    }
	
	// Barycentric interpolation between points
	friend Point2D lerp(Point2D from, Point2D* to, Scalar* weight, int toCount)
	{
		Vector2D sum;

		for( int i=0; i<toCount; ++i )
		{
			sum += (to[i]-from) * weight[i];
		}

		return from + sum;
	}

    // Difference
    // Usage: Vector2D v = p - q;
    //   where p and q are points
    friend Vector2D operator- (Point2D p, Point2D q)
    {
        return Vector2D(p.x-q.x, p.y-q.y);
    }

    // Translate this point inplace
    // Usage: p += v;
    //   where p is a point and and v is a vector
    void operator += (Vector2D v)
    {
        x += v.x;
        y += v.y;
    }

    // Translate this point inplace (opposite direction)
    // Usage: p -= v;
    //   where p is a point and and v is a vector
    void operator -= (Vector2D v)
    {
        x -= v.x;
        y -= v.y;
    }

    // Are two points equal within a threshold?
    // Usage: p.Equals(q)
    //   where p and q are points
    bool Equals(Point2D p, Scalar epsilon=0.001)
    {
        return isEqual(x, p.x, epsilon) && isEqual(y, p.y, epsilon);
    }

    // Are two points exactly equal?
    // Usage: p == q
    //   where p and q are points
    friend bool operator == (Point2D p, Point2D q)
    {
        return p.x == q.x && p.y == q.y;
    }

    // Are two points exactly unequal?
    // Usage: p != q
    //   where p and q are points
    friend bool operator != (Point2D p, Point2D q)
    {
        return p.x != q.x || p.y != q.y;
    }

    // Distance between two points.
    friend Scalar distanceBetween(Point2D p, Point2D q)
    {
        return norm(p-q);
    }

    // Distance from this point to that point.
    Scalar DistanceTo(Point2D that) { return distanceBetween(*this, that); }

    // Convert from Direct2D
    Point2D(const D2D1_POINT_2F& p):x(p.x), y(p.y)
    {
    }

    // Convert to Direct2D
    operator D2D1_POINT_2F() const
    {
        D2D1_POINT_2F p;
        p.x = float(x);
        p.y = float(y);
        return p;
    }
   // Converteert dit punt naar een String in de vorm [x,y]
   String ToString() const;

	// Print to a stream.
	friend std::ostream& operator<<(std::ostream& os, Point2D p) 
	{
		os << "Point2D( " << p.x << ", " << p.y << " )";
		return os;
	}

	friend bool IntersectLineSegments(
		Point2D p1, Point2D p2, 
		Point2D q1, Point2D q2, 
		Scalar& outLambda1, Scalar& outLambda2, 
		Scalar epsilon = 1e-6)
	{
		bool hit = IntersectLines(p1,p2,q1,q2,outLambda1, outLambda2, epsilon);

		if( hit )
		{
			hit = 
				outLambda1 >= 0 && outLambda1 <= 1 &&
				outLambda2 >= 0 && outLambda2 <= 1;
		}

		return hit;
	}

	friend bool IntersectLines(
		Point2D p1, Point2D p2, 
		Point2D q1, Point2D q2, 
		Scalar& outLambda1, Scalar& outLambda2, 
		Scalar epsilon = 1e-6)
	{
		bool hit = false;

		Vector2D p1p2 = p2-p1;
		Vector2D q1q2 = q2-q1;

		Scalar denom = det(p1p2,q1q2);

		// Don't divide by zero
		if( denom < -epsilon || 
			denom > +epsilon )
		{
			hit = true;

			Vector2D p1q1 = q1-p1;

			Scalar nomin1 = det(p1q1,q1q2);
			Scalar nomin2 = det(p1q1,p1p2);
			outLambda1 = nomin1 / denom;
			outLambda2 = nomin2 / denom;
		}

		return hit;
	}
};
