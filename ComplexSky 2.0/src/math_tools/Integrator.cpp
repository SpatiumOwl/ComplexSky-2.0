#include "Integrator.h"
#include "Interpolator.h"
#include <iostream>
#include <utility>

namespace cs
{
	namespace math_tools
	{
		std::pair<double, std::vector<double>> Integrator::FindIntersection(
			std::vector<double> func1, double distance1,
			std::vector<double> func2, double distance2)
		{
			std::pair<double, std::pair<std::vector<double>, std::vector<double>>> functions =
				BringToScale(func1, distance1, func2, distance2);

			double distance = functions.first;
			std::vector<double> newFunc1 = functions.second.first;
			std::vector<double> newFunc2 = functions.second.second;

			std::vector<double> intersection = std::vector<double>(newFunc1.size());

			for (int i = 0; i < intersection.size(); i++)
				intersection[i] = fmin(newFunc1[i], newFunc2[i]);

			return std::pair<double, std::vector<double>>(distance, intersection);
		}

		std::pair<double, std::pair<std::vector<double>, std::vector<double>>> Integrator::BringToScale(
			std::vector<double> func1, double distance1,
			std::vector<double> func2, double distance2)
		{
			std::pair<double, std::pair<std::vector<double>, std::vector<double>>> 
				sameDistanceFunctions = BringToSameDistance(
				func1, distance1, func2, distance2);

			double distance = sameDistanceFunctions.first;
			std::pair<std::vector<double>, std::vector<double>> sameSizeFunctions =
				BringToSameSize(sameDistanceFunctions.second.first, sameDistanceFunctions.second.second);

			return std::pair<double, std::pair<std::vector<double>, std::vector<double>>>
				(distance, sameSizeFunctions);
		}

		std::pair<double, std::pair<std::vector<double>, std::vector<double>>> Integrator::BringToSameDistance(
			std::vector<double> func1, double distance1,
			std::vector<double> func2, double distance2)
		{
			std::vector<double> prepFunc1, prepFunc2;
			double distance;

			if (distance1 < distance2)
			{
				prepFunc1 = *(&func1);
				prepFunc2 = ConvertToNewDistance(func2, distance2, distance1);
				distance = distance1;
			}
			else if (distance1 > distance2)
			{
				prepFunc2 = *(&func2);
				prepFunc1 = ConvertToNewDistance(func1, distance1, distance2);
				distance = distance2;
			}
			else
			{
				prepFunc1 = *(&func1);
				prepFunc2 = *(&func2);
				distance = distance1;
			}

			return std::pair<double, std::pair<std::vector<double>, std::vector<double>>>
				(distance, std::pair<std::vector<double>, std::vector<double>>(prepFunc1, prepFunc2));
		}

		std::vector<double> Integrator::ConvertToNewDistance(
			std::vector<double> func, double oldDistance,
			double newDistance)
		{
			double funcLength = func.size() * oldDistance;
			int newSize = (int)std::floor(funcLength / newDistance);

			std::vector<double> newFunc = std::vector<double>(newSize);

			for (int i = 0; i < newSize; i++)
			{
				double position = (double)i / (newSize - 1.0) * (func.size() - 1.0);
				int pos0 = (int)std::floor(position);
				int pos1 = pos0 + 1;
				double relDist = position - pos0;

				double value = Interpolator::LinearInterpolate(pos0, pos1, relDist);

				newFunc[i] = value;
			}

			return newFunc;
		}

		std::pair<std::vector<double>, std::vector<double>> Integrator::BringToSameSize(
			std::vector<double> func1, std::vector<double> func2)
		{
			if (func1.size() == func2.size())
				return std::pair<std::vector<double>, std::vector<double>>(func1, func2);
			else
			{
				std::vector<double> unchangedFunc, extendedFunc;
				if (func1.size() > func2.size())
				{
					unchangedFunc = *(&func1);
					extendedFunc = *(&func2);
				}
				else
				{
					unchangedFunc = *(&func2);
					extendedFunc = *(&func1);
				}

				double lastValue = extendedFunc.back();

				while (extendedFunc.size() < unchangedFunc.size())
					extendedFunc.push_back(lastValue);

				return std::pair<std::vector<double>, std::vector<double>>(unchangedFunc, extendedFunc);
			}
		}

		double Integrator::FindArea(std::vector<double> func, double distance)
		{
			double result = 0;

			for (std::vector<double>::iterator iter = func.begin(); iter + 1 < func.end(); iter++)
				result += (*iter + *(iter + 1)) * 0.5 * distance;

			return result;
		}
	}
}