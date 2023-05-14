#pragma once
#include <string>
#include "composite_processing/RGBACompositeImage.h"
#include "composite/Layers.h"


namespace cs
{
	namespace exporters
	{
		class RGBAComposer
		{
		private:
			static void ComposeImage(
				composite_processing::RGBACompositeImage* compositeImage,
				cv::Mat4d* destination);

			static void ComposeLayer(composite::ImageLayer<cv::Mat4d>* destination,
				composite::Layer<cv::Mat4d>* layer);

			static void BlendLayer(
				composite::ImageLayer<cv::Mat4d>* source,
				composite::ImageLayer<cv::Mat4d>* destination);

			static std::pair<int, int> CalculateNewOffset(
				cs::composite::ImageLayer<cv::Mat4d>* source, 
				cs::composite::ImageLayer<cv::Mat4d>* destination);

			static cv::Vec2i CalculateNewImageSize(
				cs::composite::ImageLayer<cv::Mat4d>* source,
				cs::composite::ImageLayer<cv::Mat4d>* destination,
				std::pair<int, int>& localOffset);

			static cv::Vec4d BlendPixels(
				cs::composite::ImageLayer<cv::Mat4d>* source,
				cs::composite::ImageLayer<cv::Mat4d>* destination,
				int row, int col, std::pair<int, int>& localOffset);

			static cv::Vec4d GetPixel(
				cs::composite::ImageLayer<cv::Mat4d>* image,
				int row, int col);

			static cv::Vec4d PorterDuffFunc(
				cv::Vec4d sourcePixel, cv::Vec4d destinationPixel,
				composite::BlendingMode blendingMode);

			static cv::Vec3d GetColor(cv::Vec4d pixel);

			static double GetAlpha(cv::Vec4d pixel);

			static cv::Vec3d BlendFunc(
				cv::Vec3d colorSource, cv::Vec3d colorDestination,
				composite::BlendingMode blendingMode);
		public:
			static void ExportImage(
				composite_processing::RGBACompositeImage* compositeImage,
				std::string path);
		};
	}
}