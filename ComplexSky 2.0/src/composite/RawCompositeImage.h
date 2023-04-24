#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/Layers.h"
#include "composite/RawImage.h"

namespace cs
{
	namespace composite
	{
		class RawCompositeImage
		{
		private:
			FolderLayer<RawImage> mainFolder;
			cv::Vec2i resolutionPx;
		public:
			color_spectrum::ColorSpectrum* colorSpectrum;
			bool limitedDynamicRange;
			cv::Vec2d dynamicRange;

			RawCompositeImage();
			inline void PushLayer(Layer<RawImage>* layer)
			{
				mainFolder.layers->push_back(layer);
			}
			inline void SetPixelSize(unsigned int width, unsigned int height)
			{
				if (width > 0 && height > 0) resolutionPx = cv::Vec2i(height, width);
			}
			inline cv::Vec2i GetPixelSize() { return resolutionPx; }
		};
	}
}