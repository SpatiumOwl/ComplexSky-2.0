#pragma once
#include "color_spectrum/ColorSpectrum.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

namespace cs
{
	namespace composite
	{
		struct RawImage
		{
			color_spectrum::ColorSpectrum colorSpectrum;
			bool limitedDynamicRange;
			cv::Vec2d dynamicRange;
			cv::Vec2i resolutionPx;
			std::vector<std::vector<double>> image;
		};
	}
}
