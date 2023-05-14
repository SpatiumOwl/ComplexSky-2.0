#pragma once
#include<string>
#include<vector>

namespace cs
{
	namespace color_spectrum
	{
		struct ColorSpectrumChannel
		{
			std::vector<double> func;
			double distanceBetweenValues = 0;
		};
	}
}