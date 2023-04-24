#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/RawImage.h"


namespace cs
{
	namespace rtc
	{
		class RGBAToRawConverter
		{
			composite::RawImage Convert(cv::Mat4d image, cv::Vec2d dynamicRange);
		};
	}
}