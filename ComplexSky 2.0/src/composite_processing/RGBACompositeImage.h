#pragma once
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/Layers.h"

namespace cs
{
	namespace composite_processing
	{
		class RGBACompositeImage
		{
		private:
			std::pair<unsigned int, unsigned int> resolutionPx;
		public:
			composite::FolderLayer<cv::Mat4d> mainFolder;
			
			RGBACompositeImage();

			inline void PushLayer(composite::Layer<cv::Mat4d>* layer)
			{
				mainFolder.layers->push_back(layer);
			}

			inline void SetPixelSize(unsigned int width, unsigned int height)
			{
				if (width > 0 && height > 0) resolutionPx = std::pair<unsigned int, unsigned int>(height, width);
			}
			inline void SetPixelSize(std::pair<unsigned int, unsigned int> size)
			{
				if (size.first > 0 && size.second > 0) resolutionPx = size;
			}
			inline std::pair<unsigned int, unsigned int> GetPixelSize(unsigned int width, unsigned int height)
			{
				return resolutionPx;
			}
		};
	}
}