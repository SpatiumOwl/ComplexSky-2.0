#pragma once
#include "spectrum_db/ColorSpectrum.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

namespace cs
{
	namespace composite
	{
		struct RawImage
		{
			spectrum_db::ColorSpectrum colorSpectrum;
			bool limitedDynamicRange;
			cv::Vec2d dynamicRange;
			cv::Vec2i resolutionPx;
			std::vector<std::vector<double>> image;
		};
	}
}
