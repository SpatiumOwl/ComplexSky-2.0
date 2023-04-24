#pragma once
#include<string>
#include<vector>

namespace cs
{
	namespace spectrum_db
	{
		struct ColorSpectrum
		{
			std::string name;
			std::vector<double> func;
			double distanceBetweenValues;
		};
	}
}
