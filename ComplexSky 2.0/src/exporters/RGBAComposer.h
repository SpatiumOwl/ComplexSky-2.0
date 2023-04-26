#pragma once
#include <string>
#include "composite_processing/RGBACompositeImage.h"
#include "composite/Layers.h"


namespace cs
{
	namespace exporters
	{
		static class RGBAComposer
		{
		private:
			static void ComposeImage(
				composite_processing::RGBACompositeImage compositeImage,
				cv::Mat4d* destination);
			static void ComposeLayer(composite::ImageLayer<cv::Mat4d>* destination,
				composite::Layer<cv::Mat4d>* layer);
			static void BlendLayer(composite::ImageLayer<cv::Mat4d>* source,
				composite::ImageLayer<cv::Mat4d>* destination);
		public:
			static void ExportImage(
				composite_processing::RGBACompositeImage compositeImage,
				std::string path);
		};
	}
}