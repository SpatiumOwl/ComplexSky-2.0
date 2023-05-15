#include "RGBAComposer.h"

namespace cs
{
	namespace exporters
	{
		void RGBAComposer::ExportImage(
			composite_processing::RGBACompositeImage* compositeImage,
			std::string path)
		{
			cv::Mat4d* result = ComposeImage(compositeImage);

			cv::Mat result_8u4c;
			cv::normalize(*result, result_8u4c, 0, 255, cv::NORM_MINMAX, CV_8UC4);

			cv::imshow("Win", result_8u4c);
			cv::waitKey(0);

			cv::imwrite(path, result_8u4c);
		}

		cv::Mat4d* RGBAComposer::ComposeImage(
			composite_processing::RGBACompositeImage* compositeImage)
		{
			composite::ImageLayer<cv::Mat4d>* result = new composite::ImageLayer<cv::Mat4d>();
			result->blendingMode = composite::BlendingMode::CS_NORMAL;
			result->name = "Result";
			result->offset = std::pair<int, int>(0, 0);

			cv::Mat4d* image = new cv::Mat4d(compositeImage->GetPixelSize().second,
				compositeImage->GetPixelSize().first, CV_32FC4);

			for (auto pixel = image->begin(); pixel != image->end(); pixel++)
				*pixel = cv::Vec4d(0, 0, 0, 1);

			result->SetImage(image);

			ComposeLayer(result, compositeImage->mainFolder);

			cv::Vec4d a = result->GetImage()->at<cv::Vec4d>(0, 0);
			cv::Vec4d b = result->GetImage()->at<cv::Vec4d>(300, 300);
			cv::Vec4d c = result->GetImage()->at<cv::Vec4d>(500, 500);

			return result->GetImage();
		}

		void RGBAComposer::ComposeLayer(composite::ImageLayer<cv::Mat4d>* destination, 
			composite::Layer<cv::Mat4d>* layer)
		{
			if (!layer->IsFolder())
				BlendLayer(static_cast<composite::ImageLayer<cv::Mat4d>*>(layer), destination);
			else
			{
				composite::ImageLayer<cv::Mat4d> image;
				image.blendingMode = layer->blendingMode;
				image.name = layer->name;

				composite::FolderLayer<cv::Mat4d>* folder = static_cast<composite::FolderLayer<cv::Mat4d>*>(layer);

				for (auto curLayer = folder->layers->begin(); curLayer != folder->layers->end(); curLayer++)
					ComposeLayer(&image, *curLayer);

				BlendLayer(&image, destination);
			}
			cv::Vec4d a = destination->GetImage()->at<cv::Vec4d>(0, 0);
			cv::Vec4d b = destination->GetImage()->at<cv::Vec4d>(300, 300);
			cv::Vec4d c = destination->GetImage()->at<cv::Vec4d>(500, 500);
		}

		void RGBAComposer::BlendLayer(
			composite::ImageLayer<cv::Mat4d>* source, 
			composite::ImageLayer<cv::Mat4d>* destination)
		{
			std::pair<int, int> localOffset = CalculateNewOffset(source, destination);

			cv::Vec2i size = CalculateNewImageSize(source, destination, localOffset);

			cv::Mat4d* result = new cv::Mat4d(size[1], size[0], CV_32FC4);

			for (int i = 0; i < result->rows; i++)
				for (int j = 0; j < result->cols; j++)
					result->at<cv::Vec4d>(j, i) = BlendPixels(source, destination, i, j, localOffset);

			destination->SetImage(result);
			destination->offset = localOffset;
		}
		
		std::pair<int, int> RGBAComposer::CalculateNewOffset(
			cs::composite::ImageLayer<cv::Mat4d>* source, 
			cs::composite::ImageLayer<cv::Mat4d>* destination)
		{
			std::pair<int, int> localOffset(0, 0);
			if (destination->IsEmpty())
				localOffset = source->offset;
			else
			{
				if (source->offset.first < destination->offset.first)
					localOffset.first = source->offset.first;
				else
					localOffset.first = destination->offset.first;
				if (source->offset.second < destination->offset.second)
					localOffset.second = source->offset.second;
				else
					localOffset.second = destination->offset.second;
			}

			return localOffset;
		}

		cv::Vec2i RGBAComposer::CalculateNewImageSize(
			cs::composite::ImageLayer<cv::Mat4d>* source,
			cs::composite::ImageLayer<cv::Mat4d>* destination,
			std::pair<int, int>& localOffset)
		{
			cv::Vec2i size(0, 0);

			if (destination->IsEmpty())
				size = cv::Vec2i(source->GetImage()->cols, source->GetImage()->rows);
			else
			{
				if (source->GetImage()->cols + source->offset.first <
					destination->GetImage()->cols + destination->offset.first)
					size[0] = destination->GetImage()->cols + destination->offset.first;
				else
					size[0] = source->GetImage()->cols + source->offset.first;

				if (source->GetImage()->rows + source->offset.second <
					destination->GetImage()->rows + destination->offset.second)
					size[1] = destination->GetImage()->rows + destination->offset.second;
				else
					size[1] = source->GetImage()->rows + source->offset.second;

				size -= cv::Vec2i(localOffset.first, localOffset.second);
			}

			return size;
		}

		cv::Vec4d RGBAComposer::BlendPixels(
			cs::composite::ImageLayer<cv::Mat4d>* source, 
			cs::composite::ImageLayer<cv::Mat4d>* destination, 
			int row, int col, std::pair<int, int>& localOffset)
		{
			cv::Vec4d sourcePixel = GetPixel(source, row + localOffset.second, col + localOffset.first); 
			cv::Vec4d destinationPixel = GetPixel(destination, row + localOffset.second, col + localOffset.first);

			return PorterDuffFunc(sourcePixel, destinationPixel, source->blendingMode);
		}

		cv::Vec4d RGBAComposer::GetPixel(
			cs::composite::ImageLayer<cv::Mat4d>* image,
			int row, int col)
		{
			if (!image->IsEmpty() &&
				col >= image->offset.first &&
				col < image->offset.first + image->GetImage()->cols &&
				row >= image->offset.second &&
				row < image->offset.second + image->GetImage()->rows)
				return image->GetImage()->at<cv::Vec4d>(row - image->offset.second, col - image->offset.first);
			else
				return cv::Vec4d(0, 0, 0, 0);
		}

		cv::Vec4d RGBAComposer::PorterDuffFunc(
			cv::Vec4d sourcePixel, cv::Vec4d destinationPixel,
			composite::BlendingMode blendingMode)
		{
			cv::Vec3d Cs = GetColor(sourcePixel);
			double As = GetAlpha(sourcePixel);

			cv::Vec3d Cd = GetColor(destinationPixel);
			double Ab = GetAlpha(destinationPixel);

			double Ar = Ab + As - Ab * As;

			cv::Vec3d B = BlendFunc(Cs, Cd, blendingMode);
			cv::Vec3d Cr = (1 - As / Ar) * Cd + As / Ar * ((1 - Ab) * Cs + Ab * B);

			return cv::Vec4d(Cr[0], Cr[1], Cr[2], Ar);
		}

		cv::Vec3d RGBAComposer::GetColor(cv::Vec4d pixel)
		{
			double alpha = pixel[3];
			cv::Vec3d color;

			if (alpha != 0)
				return cv::Vec3d(pixel[0], pixel[1], pixel[2]);
			else
				return cv::Vec3d(0, 0, 0);
		}

		double RGBAComposer::GetAlpha(cv::Vec4d pixel)
		{
			return pixel[3];
		}		
		
		cv::Vec3d RGBAComposer::BlendFunc(
			cv::Vec3d colorSource, cv::Vec3d colorDestination, 
			composite::BlendingMode blendingMode)
		{
			switch (blendingMode)
			{
			case composite::BlendingMode::CS_NORMAL:
				return colorSource;
			case composite::BlendingMode::CS_SCREEN:
				return colorDestination + colorSource - colorSource.mul(colorDestination);
			case composite::BlendingMode::CS_MULTIPLY:
				return colorSource.mul(colorDestination);
			default:
				return 0;
			}
		}
	}
}