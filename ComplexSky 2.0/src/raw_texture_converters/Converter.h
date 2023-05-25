#pragma once
#include <utility>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/RawImage.h"


namespace cs
{
	namespace rtc
	{
		class Converter
		{
		public:
			virtual composite::RawImage* ImportTexture(
				std::string path, std::pair<double, double> dynamicRange) = 0;

		protected:
			static composite::RawImage* ImportImage(
				std::string path, std::pair<double, double> dynamicRange, int colorChannelNumber);
		};
	}
}