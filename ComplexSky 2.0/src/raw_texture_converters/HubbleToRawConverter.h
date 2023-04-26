#pragma once
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/RawImage.h"


namespace cs
{
	namespace rtc
	{
		class HubbleToRawConverter
		{
			composite::RawImage Convert(cv::Mat4d image, std::pair<double, double> dynamicRange);
		};
	}
}