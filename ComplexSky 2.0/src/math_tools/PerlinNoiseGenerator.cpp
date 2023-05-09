#include "PerlinNoiseGenerator.h"
#include "Interpolator.h"
#include <math.h>
#include <iostream>

namespace cs
{
	namespace math_tools
	{
		double PerlinNoiseGenerator::Noise2f(int x, int y)
		{
			int n = x + y * 57;
			return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 2147483647) / 1073741824.0f;
		}
		double PerlinNoiseGenerator::SmoothNoise2f(int x, int y)
		{
			double corners = (Noise2f(x - 1, y - 1) + Noise2f(x + 1, y - 1) + Noise2f(x - 1, y + 1) + Noise2f(x + 1, y + 1)) / 16;
			double sides = (Noise2f(x - 1, y) + Noise2f(x + 1, y) + Noise2f(x, y - 1) + Noise2f(x, y + 1)) / 8;
			double center = Noise2f(x, y) / 4;
			return corners + sides + center;
		}
		
		double PerlinNoiseGenerator::LinearInterpolatedNoise2f(double x, double y)
		{
			int integer_X = (int)std::floor(x);
			double fractional_X = x - integer_X;
			int integer_Y = (int)std::floor(y);
			double fractional_Y = y - integer_Y;
			double v00 = SmoothNoise2f(integer_X, integer_Y);
			double v01 = SmoothNoise2f(integer_X + 1, integer_Y);
			double v10 = SmoothNoise2f(integer_X, integer_Y + 1);
			double v11 = SmoothNoise2f(integer_X + 1, integer_Y + 1);

			return Interpolator::LinearInterpolate2f(
				v00, v10,
				v01, v11, 
				fractional_X, fractional_Y);
		}
		double PerlinNoiseGenerator::SquareInterpolatedNoise2f(double x, double y)
		{
			int integer_X = (int)std::floor(x);
			double fractional_X = x - integer_X;
			int integer_Y = (int)std::floor(y);
			double fractional_Y = y - integer_Y;
			double v00 = SmoothNoise2f(integer_X - 1, integer_Y - 1);
			double v10 = SmoothNoise2f(integer_X, integer_Y - 1);
			double v20 = SmoothNoise2f(integer_X + 1, integer_Y - 1);
			double v01 = SmoothNoise2f(integer_X - 1, integer_Y);
			double v11 = SmoothNoise2f(integer_X, integer_Y);
			double v21 = SmoothNoise2f(integer_X + 1, integer_Y);
			double v02 = SmoothNoise2f(integer_X - 1, integer_Y + 1);
			double v12 = SmoothNoise2f(integer_X, integer_Y + 1);
			double v22 = SmoothNoise2f(integer_X + 1, integer_Y + 1);

			return Interpolator::SquareInterpolate2f(
				v00, v10, v20,
				v01, v11, v21,
				v02, v12, v22,
				fractional_X, fractional_Y);
		}
		double PerlinNoiseGenerator::LinearNoise2D(double x, double y, double persistence, int numberOfOctaves, double seed)
		{
			double total = 0;
			for (int i = 0; i < numberOfOctaves; i++)
			{
				int frequency = 2 ^ i;
				double amplitude = std::pow(persistence, i);
				total += LinearInterpolatedNoise2f(x * frequency + seed, y * frequency + seed) * amplitude;
			}
			if (total < 0)
				total = 0;
			return total;
		}
		double PerlinNoiseGenerator::SquareNoise2D(double x, double y, double persistence, int numberOfOctaves, double seed)
		{
			double total = 0, maxPossibleVal = 0;
			for (int i = 0; i < numberOfOctaves; i++)
			{
				int frequency = 2 ^ i;
				double amplitude = std::pow(persistence, i);
				total += SquareInterpolatedNoise2f(x * frequency + seed, y * frequency + seed) * amplitude;
				maxPossibleVal += amplitude;
			}
			total /= maxPossibleVal;
			return total;
		}
	}
}