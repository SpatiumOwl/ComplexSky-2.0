#include "Interpolator.h"
#include <math.h>
#include <iostream>

namespace cs
{
	namespace math_tools
	{
		double Interpolator::LinearInterpolate(double a, double b, double relDist)
		{
			return a + (b - a) * relDist; //a + (b-a)*x
		}
		double Interpolator::SquareInterpolate(double a, double b, double c, double relDist)
		{
			return 0.5 * (a + c - 2 * b) * relDist * relDist + 0.5 * (c - a) * relDist + b;
		}
		double Interpolator::SquareInterpolate2f(
			double v00, double v10, double v20,
			double v01, double v11, double v21,
			double v02, double v12, double v22,
			double relDistX, double relDistY)
		{
			double i1 = SquareInterpolate(v00, v10, v20, relDistX);
			double i2 = SquareInterpolate(v01, v11, v21, relDistX);
			double i3 = SquareInterpolate(v02, v12, v22, relDistX);

			return SquareInterpolate(i1, i2, i3, relDistY);
		}

		double Interpolator::LinearInterpolate2f(
			double v00, double v10,
			double v01, double v11,
			double relDistX, double relDistY)
		{
			double i1 = LinearInterpolate(v00, v10, relDistX);
			double i2 = LinearInterpolate(v01, v11, relDistX);

			return LinearInterpolate(i1, i2, relDistY);
		}
	}
}