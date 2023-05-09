#pragma once
namespace cs
{
    namespace math_tools
    {
        class PerlinNoiseGenerator
        {
        private:
            static double Noise2f(const int x, const int y);
            static double SmoothNoise2f(const int x, const int y);

            static double LinearInterpolatedNoise2f(const double x, const double y);
            static double SquareInterpolatedNoise2f(const double x, const double y);
        public:
            static double LinearNoise2D(const double x, const double y, const double persistence, const int numberOfOctaves, const double seed);
            static double SquareNoise2D(const double x, const double y, const double persistence, const int numberOfOctaves, const double seed);
        };
    }
}