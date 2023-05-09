#pragma once
namespace cs
{
    namespace math_tools
    {
        class Interpolator
        {
        public:
            static double LinearInterpolate(const double a, const double b, const double relDist);

            static double SquareInterpolate(double a, double b, double c, double relDist);

            static double SquareInterpolate2f(
                double v00, double v10, double v20,
                double v01, double v11, double v21,
                double v02, double v12, double v22,
                double relDistX, double relDistY);

            static double LinearInterpolate2f(
                double v00, double v10,
                double v01, double v11, 
                double relDistX, double relDistY);
        };
    }
}

