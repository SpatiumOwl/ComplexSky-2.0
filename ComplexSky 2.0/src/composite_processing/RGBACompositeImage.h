#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "composite/Layers.h"

namespace cs
{
	namespace composite_processing
	{
		class RGBACompositeImage
		{
		public:
			composite::FolderLayer<cv::Mat4d> mainFolder;
			cv::Vec2i resolutionPx;
			
			RGBACompositeImage(cv::Vec2i _resolutionPx);
			inline void PushLayer(composite::Layer<cv::Mat4d>* layer)
			{
				mainFolder.layers->push_back(layer);
			}
			inline void SetPixelSize(unsigned int width, unsigned int height)
			{
				if (width > 0 && height > 0) resolutionPx = cv::Vec2i(height, width);
			}
		};
	}
}