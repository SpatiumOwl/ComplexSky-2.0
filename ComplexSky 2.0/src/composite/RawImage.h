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
			color_spectrum::ColorSpectrum* colorSpectrum = nullptr;
			bool limitedDynamicRange = false;
			std::pair<double, double> dynamicRange;
			std::pair<unsigned int, unsigned int> resolutionPx;
			std::vector<std::vector<double>> image;
			std::vector<double> alpha;

			inline RawImage(color_spectrum::ColorSpectrum* _colorSpectrum,
				bool _limitedDynamicRange,
				std::pair<double, double> _dynamicRange,
				std::pair<unsigned int, unsigned int> _resolutionPx)
			{
				colorSpectrum = _colorSpectrum;
				limitedDynamicRange = _limitedDynamicRange;
				dynamicRange = _dynamicRange;
				resolutionPx = _resolutionPx;

				alpha = std::vector<double>(resolutionPx.first * resolutionPx.second, 1);
				image = std::vector<std::vector<double>>(resolutionPx.first * resolutionPx.second, 
					std::vector<double>(colorSpectrum->colorChannels.size(), 0));
			}
			inline RawImage(bool _limitedDynamicRange,
				std::pair<double, double> _dynamicRange,
				std::pair<unsigned int, unsigned int> _resolutionPx,
				int colorChannelNumber)
			{
				limitedDynamicRange = _limitedDynamicRange;
				dynamicRange = _dynamicRange;
				resolutionPx = _resolutionPx;

				alpha = std::vector<double>(resolutionPx.first * resolutionPx.second, 1);
				image = std::vector<std::vector<double>>(resolutionPx.first * resolutionPx.second,
					std::vector<double>(colorChannelNumber, 0));
			}
			inline RawImage() {}
		};
	}
}
