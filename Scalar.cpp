#include "StdAfx.h"
#include "Scalar.h"

static double DBL_NAN = sqrt(-1.0);

int solveQuadratic(Scalar a, Scalar b, Scalar c, Scalar* x)
{
	int numberOfXs = 0;

	Scalar epsilon = 1e-6;

	if( fabs(a) < epsilon )
	{
		x[0] = x[1] = solveLinear(b,c);
		numberOfXs = 1;
	}
	else
	{
		// Compute the discriminant d
		Scalar d = b*b - 4*a*c;

		if( d < 0 )
		{
			// No real solutions
			x[0] = x[1] = DBL_NAN;
			numberOfXs = 0;
		}
		else if( fabs(d) < epsilon )
		{
			// One real solution
			x[0] = x[1] = -b / (2*a);
			numberOfXs = 1;
		}
		else
		{
			// Two real solutions
			x[0] = (-b + sqrt(d)) / (2*a);
			x[1] = (-b - sqrt(d)) / (2*a);
			numberOfXs = 2;
		}
	}

	return numberOfXs;
}
