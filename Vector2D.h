#pragma once

#include "Scalar.h"
#include "Angle.h"

// A 2D vector class
//
// NOTE: This class is designed for math educational purposes only. 
// The vector class of a real game engine would use all kinds of optimization tricks,
// but this would make the code harder to read. However, a good C++ compiler will usually 
// optimize the hell out of the code anyway, so in practice, this design is not as bad as it looks.
//
// This class has two types of member functions:
// (1) static friend  member functions that start with a lowercase letter, like dot(a,b)
// (2) class member functions that start with an uppercase letter, like a.Dot(b)
//
// It is matter of taste what to use. E.g. some people prefer a.Length() * b.Dot(c), 
// others length(a) * dot(b,c), others length(a) * b.Dot(c). 

struct Vector2D
{
	// The first coordinate (relative to some basis).
	Scalar x;

	// The second coordinate (relative to some basis).
	Scalar y;

	// Default constructor gives the zero vector.
	Vector2D() : x(0), y(0) {}

	// Constructor
	// Usage: Vector2D v(1,2);
	Vector2D(Scalar x, Scalar y) : x(x), y(y) {}

	// Addition
	// Usage: Vector2D w = u + v;
	//   where u and v are vectors
	friend Vector2D operator+ (Vector2D a, Vector2D b)
	{
		return Vector2D(a.x+b.x, a.y+b.y); 
	}

	// Subtraction
	// Usage: Vector2D w = u - v;
	//   where u and v are vectors
	friend Vector2D operator- (Vector2D a, Vector2D b)
	{
		return Vector2D(a.x-b.x, a.y-b.y); 
	}

	// Negation
	// Usage: Vector2D w = -v;
	//   where v is a vector
	friend Vector2D operator- (Vector2D a)
	{
		return Vector2D(-a.x, -a.y); 
	}

	// Identity
	// Usage: Vector2D w = +v;
	//   where v is a vector
	friend Vector2D operator+ (Vector2D a)
	{
		return a;
	}

	// Right multiplication with a scalar
	// Usage: Vector2D w =v * 2;
	//   where v is a vector
	friend Vector2D operator* (Vector2D a, Scalar s)
	{
		return Vector2D(a.x*s, a.y*s); 
	}

	// Left multiplication with a scalar
	// Usage: Vector2D w = 2 * v;
	//   where v is a vector
	friend Vector2D operator* (Scalar s, Vector2D a)
	{
		return Vector2D(a.x*s, a.y*s); 
	}

	// Division by scalar
	// Usage: Vector2D w = v/2;
	//   where v is a vector
	friend Vector2D operator/ (Vector2D a, Scalar s)
	{
		return Vector2D(a.x/s, a.y/s); 
	}

	// Add vector inplace
	// Usage: v += w;
	//   where v and w are vectors
	void operator += (Vector2D v)
	{
		x += v.x;
		y += v.y;
	}

	// Subtract vector inplace
	// Usage: v -= w;
	//   where v and w are vectors
	void operator -= (Vector2D v)
	{
		x -= v.x;
		y -= v.y;
	}

	// Scale vector inplace
	// Usage: v *= s;
	//   where v is a vector and s is a scalar
	void operator *= (Scalar s)
	{
		x *= s;
		y *= s;
	}

	// Downscale vector inplace
	// Usage: v /= s;
	//   where v is a vector and s is a scalar
	void operator /= (Scalar s)
	{
		x /= s;
		y /= s;
	}

    // Are two vectors equal within a threshold?
    // Usage: p.Equals(q)
    //   where p and q are vectors
    bool Equals(Vector2D p, Scalar epsilon=0.001)
    {
        return isEqual(x, p.x, epsilon) && isEqual(y, p.y, epsilon);
    }

    // Are two vectors exactly equal?
    // Usage: p == q
    //   where p and q are vectors
    friend bool operator == (Vector2D p, Vector2D q)
    {
        return p.x == q.x && p.y == q.y;
    }

    // Are two vectors exactly unequal?
    // Usage: p != q
    //   where p and q are vectors
    friend bool operator != (Vector2D p, Vector2D q)
    {
        return p.x != q.x || p.y != q.y;
    }


	// Dot product 
	// Usage: Scalar d = dot(u,v);
	//   where u and v are vectors
	friend Scalar dot(Vector2D a, Vector2D b)
	{ 
		return a.x*b.x + a.y*b.y;
	}

	// Determinant, i.e. the signed area of the parallelogram spanned by the vectors
	// Usage: Scalar d = det(u,v);
	//   where u and v are vectors
	friend Scalar det(Vector2D a, Vector2D b)
	{
		return a.x*b.y - a.y*b.x; 
	}

	// Norm (aka length or magnitude) of vector 
	// Usage: Scalar l = norm(v);
	//   where v is a vector
	friend Scalar norm(Vector2D a)
	{
		return sqrt(dot(a,a));
	}

	// Returns the normalized vector (aka a unit vector, i.e. vector with norm 1)
	// NOTE: If the length of the vector a is smaller than epsilon, this returns the zero vector.
	// Usage: Vector2D u = normalized(v);
	//    where v is a vector
	friend Vector2D normalized(Vector2D a, Scalar epsilon=0.001)
	{
		Scalar length = norm(a);
		return length<epsilon ? Vector2D(0,0) : a / length;
	}

	// The coordinate of the vector (v) orthogonally projected on the vector (d)
	// (d) does not need to be a unit direction.
	// NOTE: If length of (d) < (epsilon), this returns 0
	// Usage: Scalar x = orthoCoordOf(v,u);
	//    where u and v are vectors
	friend Scalar orthoCoordOf(Vector2D v, Vector2D d, Scalar epsilon)
	{
		Scalar vd = dot(v,d);
		Scalar dd = dot(d,d);
		return dd < epsilon ? 0 : vd / dd;
	}

	// Vector2D (v) orthogonally projected on vector (d).
	// (d) does not need to be a unit direction.
	// NOTE: If length of (d) < (epsilon), this returns the zero vector
	// Usage: Vector2D w = orthoProjOf(v,u);
	//    where u and v are vectors
	friend Vector2D orthoProjOf(Vector2D v, Vector2D d, Scalar epsilon=0.001)
	{
		Scalar s = orthoCoordOf(v,d,epsilon);
		return s * d;
	}

	// Vector2D orthogonal on vector (a), in same rotation sense as basis
	// NOTE: This only works in 2D, as in 3D, you have an infinite amount 
	// of orthogonal vectors (they form a disc)
	// Usage: Vector2D w = orthogonal(v);
	//    where v is a vector
	friend Vector2D orthogonal(Vector2D a) 
	{ 
		return Vector2D(-a.y, a.x); 
	}

	// Angle (in radians) of vector with first basis vector (the "x-axis")
	// Usage: Scalar a = angle(v);
	//   where v is a vector
	friend Radians angle(Vector2D a)
	{
		return atan2(a.y, a.x);
	}

	// Construct a vector from an angle (in radians) 
	// and a length (aka polar coordinates)
	// Usage: Vector2D v = Vector2D::fromPolar(M_PI/2, 10);
	static Vector2D FromPolar(Radians angle, Scalar length)
	{
		return length*Vector2D(cos(angle), sin(angle));
	}
	
	// Construct a unit vector from an angle (in radians) 
	// Usage: Vector2D v = Vector2D::fromAngle(M_PI/2, 10);
	static Vector2D FromAngle(Radians angle)
	{
		return FromPolar(angle,1);
	}

	// The angle (in radians) between two vectors
	// Usage: Scalar a = angleBetween(u,v);
	//   where u and v are vectors
	friend Radians angleBetween(Vector2D a, Vector2D b)
	{
		Scalar x = dot(a,b);
		Scalar y = det(a,b);
		return atan2(y,x);
	}

	// The vector (v) rotated by (angle) radians
	// Usage: Vector2D w = rotated(v, M_PI/2);
	//    where v is v vector
	friend Vector2D rotated(Vector2D v, Radians angle)
	{
		// Usually this is done with a matrix,
		// but I want to show you the geometrical way of doing this.
		Vector2D e1 = FromAngle(angle);
		Vector2D e2 = orthogonal(e1);
		return v.x*e1 + v.y*e2;
	}

	// The vector (v) reflected about the vector (n)
	// (n) does not have to be a unit vector.
	// Usage: Vector2D w = reflected(v, u);
	//  where v and u are vectors
	friend Vector2D reflected(Vector2D v, Vector2D n)
	{
		Vector2D von = v.On(n);
		return 2*von - v;
	}

	// Lineair interpolation between two vectors
	// Usage: Vector2D w = lerp(u,v,0.5)
	//   where u and v are vectors.
	friend Vector2D lerp(Vector2D from, Vector2D to, Scalar lambda)
	{
		return from + (to-from) * lambda;
	}
	// Dot is method form of dot
	// Usage: Scalar d = u.Dot(v);
	//    where u and v are vectors
	Scalar Dot(Vector2D that) const { return dot(*this, that); }

	// Det is method form of det
	// Usage: Scalar d = u.Det(v);
	//    where u and v are vectors
	Scalar Det(Vector2D that) const { return det(*this, that); }


	// Norm is method form of norm
	// Usage: Scalar l = v.Norm();
	//   where v is a vector
	Scalar Norm() const { return norm(*this); }

	// Length is length or magnitude) of vector 
	// Usage: Scalar l = v.Norm();
	//   where v is a vector
	Scalar Length() const { return norm(*this); }

	// Angle is method form of angle
	// Usage: Scalar a = v.Angle();
	//   where v is a vector
	Radians Angle() const { return angle(*this); }

	// AngleWidth is method form of angleBetween
	// Usage: Scalar d = u.AngleWith(v);
	//    where u and v are vectors
	Radians AngleWith(Vector2D that) const { return angleBetween(*this,that); }

	// Normalized is method form of normalized
	// Usage: Vector2D u = v.Normalized();
	//    where v is a vector
	Vector2D Normalized(Scalar epsilon=0.001) const { return normalized(*this, epsilon); }

	// OrthoCoordOn is method form of orthoCoordOn
	// Usage: Scalar x = v.OrthoCoordOn(u);
	//    where u and v are vectors
	Scalar OrthoCoordOn(Vector2D dir, Scalar epsilon=0.001) const { return orthoCoordOf(*this, dir, epsilon); }

	// On is method form of orthoProjOn
	// Usage: Vector2D w = v.On(u);
	//    where u and v are vectors
	Vector2D On(Vector2D dir, Scalar epsilon=0.001) const { return orthoProjOf(*this, dir, epsilon); }

	// Orthogonal is method form of orthogonal
	// Usage: Vector2D w = v.Orthogonal();
	//    where v is a vector
	Vector2D Orthogonal() const { return orthogonal(*this); }

	// RotatedBy is method form of rotated
	// Usage: Vector2D w = v.RotatedBy(M_PI/2);
	//    where v is v vector
	Vector2D RotatedBy(Radians angle) const { return rotated(*this, angle); }

	// Reflected is method form of reflected
	Vector2D Reflected(Vector2D about) const { return reflected(*this, about); }

	  // Converteert deze vector naar een String in de vorm [x,y]
   String ToString() const;

	// Print to a stream.
	friend std::ostream& operator<<(std::ostream& os, Vector2D v) 
	{
		os << "Vector2D( " << v.x << ", " << v.y << " )";
		return os;
	}

};
