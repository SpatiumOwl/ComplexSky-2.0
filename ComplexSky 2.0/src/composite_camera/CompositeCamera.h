#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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
			cv::Vec2d resolutionPx;
			bool limitDynamicRange;
			cv::Vec2d dynamicRange;

		private:
			void RenderLight();
			void RenderStarAmbience();
			void CreateCloudMap1D(cv::Mat1d* cloudMap);
			void CreateShadedCloudMap();

		public:
			CompositeCamera(color_spectrum::ColorSpectrum* targetSpectrum,
				cv::Vec2i resolutionPx,
				cv::Vec2d dynamicRange);
			CompositeCamera(color_spectrum::ColorSpectrum* targetSpectrum,
				cv::Vec2i resolutionPx);
			composite::RawCompositeImage Capture(sky::Sky sky,
				cv::Vec4d borderCoordinates);

		};
	}
}