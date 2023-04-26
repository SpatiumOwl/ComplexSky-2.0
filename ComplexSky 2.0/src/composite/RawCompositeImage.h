#pragma once
#include <utility>
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
			std::pair<unsigned int, unsigned int> resolutionPx;
		public:
			color_spectrum::ColorSpectrum* colorSpectrum;
			bool limitedDynamicRange;
			std::pair<double, double> dynamicRange;

			RawCompositeImage();
			inline void PushLayer(Layer<RawImage>* layer)
			{
				mainFolder.layers->push_back(layer);
			}
			inline void SetPixelSize(unsigned int width, unsigned int height)
			{
				if (width > 0 && height > 0) resolutionPx = std::pair<unsigned int, unsigned int>(height, width);
			}
			inline std::pair<unsigned int, unsigned int> GetPixelSize() { return resolutionPx; }
		};
	}
}