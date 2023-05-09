#pragma once
#include "ColorSpectrumChannel.h"
#include<string>
#include<vector>

namespace cs
{
	namespace color_spectrum
	{
		struct ColorSpectrum
		{
			std::string name;
			std::vector<ColorSpectrumChannel> colorChannels;
		};
	}
}
