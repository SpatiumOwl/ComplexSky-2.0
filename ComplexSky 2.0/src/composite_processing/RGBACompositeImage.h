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
		public:
			composite::FolderLayer<cv::Mat4d> mainFolder;
			std::pair<unsigned int, unsigned int> resolutionPx;
			
			RGBACompositeImage(std::pair<unsigned int, unsigned int> _resolutionPx);
			inline void PushLayer(composite::Layer<cv::Mat4d>* layer)
			{
				mainFolder.layers->push_back(layer);
			}
		};
	}
}