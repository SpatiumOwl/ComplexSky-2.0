#include "PerlinNoiseGenerator.h"
#include <math.h>
#include <iostream>

namespace cs
{
	namespace composite_camera
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
		double PerlinNoiseGenerator::LinearInterpolate(double a, double b, double x)
		{
			return a * (1 - x) + b * x; //a + (b-a)*x
		}
		double PerlinNoiseGenerator::SquareInterpolate(double a, double b, double c, double x)
		{
			return 0.5 * (a + c - 2 * b) * x * x + 0.5 * (c - a) * x + b;
		}
		double PerlinNoiseGenerator::LinearInterpolatedNoise2f(double x, double y)
		{
			int integer_X = (int)std::floor(x);
			double fractional_X = x - integer_X;
			int integer_Y = (int)std::floor(y);
			double fractional_Y = y - integer_Y;
			double v1 = SmoothNoise2f(integer_X, integer_Y);
			double v2 = SmoothNoise2f(integer_X + 1, integer_Y);
			double v3 = SmoothNoise2f(integer_X, integer_Y + 1);
			double v4 = SmoothNoise2f(integer_X + 1, integer_Y + 1);
			double i1 = LinearInterpolate(v1, v2, fractional_X);
			double i2 = LinearInterpolate(v3, v4, fractional_X);
			return LinearInterpolate(i1, i2, fractional_Y);
		}
		double PerlinNoiseGenerator::SquareInterpolatedNoise2f(double x, double y)
		{
			int integer_X = (int)std::floor(x);
			double fractional_X = x - integer_X;
			int integer_Y = (int)std::floor(y);
			double fractional_Y = y - integer_Y;
			double v1 = SmoothNoise2f(integer_X - 1, integer_Y - 1);
			double v2 = SmoothNoise2f(integer_X, integer_Y - 1);
			double v3 = SmoothNoise2f(integer_X + 1, integer_Y - 1);
			double v4 = SmoothNoise2f(integer_X - 1, integer_Y);
			double v5 = SmoothNoise2f(integer_X, integer_Y);
			double v6 = SmoothNoise2f(integer_X + 1, integer_Y);
			double v7 = SmoothNoise2f(integer_X - 1, integer_Y + 1);
			double v8 = SmoothNoise2f(integer_X, integer_Y + 1);
			double v9 = SmoothNoise2f(integer_X + 1, integer_Y + 1);
			double i1 = SquareInterpolate(v1, v2, v3, fractional_X);
			double i2 = SquareInterpolate(v4, v5, v6, fractional_X);
			double i3 = SquareInterpolate(v7, v8, v9, fractional_X);
			return SquareInterpolate(i1, i2, i3, fractional_Y);
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