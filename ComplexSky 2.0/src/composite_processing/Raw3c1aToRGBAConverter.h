#pragma once
#include "RGBACompositeImage.h"
#include "composite/RawCompositeImage.h"

namespace cs
{
	namespace composite_processing
	{
		static class Raw3c1aToRGBAConverter
		{
		public:
			static RGBACompositeImage ConvertRaw3c1aToRGBA
			(composite::RawCompositeImage rawCompositeImage);
		
			static cv::Mat4d ConvertRaw3c1aToRGBA
			(composite::RawImage* rawImage);

		private:
			static void ConvertLayer(
				composite::Layer<composite::RawImage>* from,
				composite::Layer<cv::Mat4d>* to);
			static void ConvertFolderLayer(
				composite::FolderLayer<composite::RawImage>* from,
				composite::FolderLayer<cv::Mat4d>* to);
			static void ConvertImageLayer(
				composite::ImageLayer<composite::RawImage>* from,
				composite::ImageLayer<cv::Mat4d>* to);
		};
	}
}