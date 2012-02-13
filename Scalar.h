#pragma once

#define NOMINMAX // We'll use min/max functions, no macros please.
#define _USE_MATH_DEFINES // Define M_PI and other constants

#include <math.h>
#include <algorithm>

// You can think of a "Scalar" as a number that is used to describe scales, lengths, distances, ...
// We define it as double, but we could just as well define it as float.
// NOTE: Always use the Scalar type in your code instead of using double or float directly. 
// Why? Ask Peter
typedef double Scalar;

// Are two scalars equal? (approximately)
inline bool isEqual(Scalar a, Scalar b, Scalar epsilon=0.000001)
{
	return abs(a-b) < epsilon;
}

// Generic modulo funtions. Works on both ints and floats.
inline int mod(int n, int d) { return n % d; }
inline float mod(float n, float d) { return fmod(n,d); }
inline double mod(double n, double d) { return fmod(n,d); }

// Strict modulo function.
// Will always return a value (m) between 0 inclusive and (d) exclusive
// In other words, 0 <= m < d
template<typename T> inline T smod(T n, T d)
{
	T m = mod(n,d);
	return m>=0 ? m : m+d;
}

// Returns (value) clamped between (minimum) and (maximum)
inline Scalar clamped(Scalar value, Scalar minimum, Scalar maximum)
{
	return std::max(std::min(value,maximum), minimum);
}

// Linear interpolation between (start) and (end) by (s).
// (s) is usually between 0 and 1, but doesn't have to be
inline Scalar lerp(Scalar start, Scalar end, Scalar s)
{
	return start + s * (end-start);
}

// A random number between (minimum) and (maximum)
inline Scalar randomBetween(Scalar minimum, Scalar maximum)
{
	return lerp(minimum, maximum, rand() / Scalar(RAND_MAX));
}

// Wrap a cyclic value (x) with lower bounds (lo) and higher bounds (hi).
// For example for the hours of a clock, lo=1 and hi=24.
// The returned value (m) will be between (lo) inclusive and (hi) exclusive 
// In other words, lo <= m < hi
inline Scalar wrap(Scalar x, Scalar lo, Scalar hi)
{
	Scalar m = smod(x-lo,hi-lo);
	return lo+m;
}
inline int wrap(int x, int lo, int hi)
{
	int m = smod(x-lo,hi-lo);
	return lo+m;
}

// Returns the sign of n.
// The is -1 for strictly negative numbers,
// otherwise +1 (for positive numbers or zero).
inline Scalar sign(Scalar n)
{
	return n>=0 ? +1 : -1;
}

// Compute the 1-dimensional between x1 and x2
inline Scalar distanceBetween( Scalar x1, Scalar x2 )
{
	Scalar dx = x2-x1;
	return fabs(dx);
}

// Compute the 2-dimensional distance between (x1,y1) and (x2,y2)
inline Scalar distanceBetween( Scalar x1, Scalar y1, Scalar x2, Scalar y2 )
{
	Scalar dx = x2 - x1;
	Scalar dy = y2 - y1;
	Scalar d = sqrt(dx*dx+dy*dy);
	return d;
}

// Signed area of parallelogram spanned by vectors (x1,y1) and (x2,y2)
inline Scalar determinant( Scalar x1, Scalar y1, Scalar x2, Scalar y2 )
{
	return x1*y2-x2*y1;
}

// Solve a linear equation in the form a*x+b = 0 in x
// It is assumed that a != 0
inline Scalar solveLinear(Scalar a, Scalar b)
{
	return -b/a;
}

// Solve the equation a*x^2 + b*x + c = 0 in x
// If a is zero, solves the linear equation b*x + c = 0.
// x should point to an array of at least 2 scalars.
// The number of solutions is returned (maximum 2)
int solveQuadratic(Scalar a, Scalar b, Scalar c, Scalar* x);
