#pragma once

#define _USE_MATH_DEFINES // Define M_PI and other constants

#include "Scalar.h"
#include <math.h>

typedef Scalar Radians;
typedef Scalar Degrees;

inline Degrees toDegrees(Radians angle)
{
	return angle * 180 / M_PI;
}

inline Radians toRadians(Degrees angle)
{
	return angle * M_PI / 180;
}
