#pragma once
#include <utility>
#include <vector>
#include "color_spectrum/ColorSpectrum.h"
#include "composite/RawCompositeImage.h"
#include "sky/Sky.h"

namespace cs
{
	namespace composite_camera
	{
		class CompositeCamera
		{
		public:
			color_spectrum::ColorSpectrum* targetSpectrum;
			std::pair<unsigned int, unsigned int> resolutionPx;
			bool limitDynamicRange;
			std::pair<double, double> dynamicRange;

		private:
			void RenderLight();
			void RenderStarAmbience();
			void CreateCloudMap1D(std::vector<std::vector<double>>* cloudMap);
			void CreateShadedCloudMap();

		public:
			CompositeCamera(color_spectrum::ColorSpectrum* targetSpectrum,
				std::pair<unsigned int, unsigned int> resolutionPx,
				std::pair<double, double> dynamicRange);
			CompositeCamera(color_spectrum::ColorSpectrum* targetSpectrum,
				std::pair<unsigned int, unsigned int> resolutionPx);
			composite::RawCompositeImage Capture(sky::Sky sky,
				std::vector<double> borderCoordinates);

		};
	}
}