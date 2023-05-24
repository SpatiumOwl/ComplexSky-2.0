#pragma once
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/RawImage.h"
#include "Converter.h"
#include "color_spectrum/ColorSpectrumDB.h"


namespace cs
{
	namespace rtc
	{
		class HubbleToRawConverter : Converter
		{
		public:
			inline composite::RawImage* ImportTexture(std::string path, std::pair<double, double> dynamicRange)
			{
				composite::RawImage* texture = ImportImage(path, dynamicRange);

				texture->colorSpectrum = 
					color_spectrum::ColorSpectrumDB::GetInstance()->GetSpectrum("Hubble");

				return texture;
			}
		};
	}
}