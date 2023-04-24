#pragma once
#include<string>
#include<vector>

namespace cs
{
	namespace color_spectrum
	{
		struct ColorSpectrum
		{
			std::string name;
			std::vector<double> func;
			double distanceBetweenValues;
		};
	}
}
