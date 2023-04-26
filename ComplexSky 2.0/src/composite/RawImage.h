#pragma once
#include "color_spectrum/ColorSpectrum.h"
#include <utility>
#include <vector>

namespace cs
{
	namespace composite
	{
		struct RawImage
		{
			color_spectrum::ColorSpectrum colorSpectrum;
			bool limitedDynamicRange;
			std::pair<double, double> dynamicRange;
			std::pair<unsigned int, unsigned int> resolutionPx;
			std::vector<std::vector<double>> image;
		};
	}
}
