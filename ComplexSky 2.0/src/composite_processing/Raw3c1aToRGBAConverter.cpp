#include "Raw3c1aToRGBAConverter.h"

namespace cs
{
	namespace composite_processing
	{
		RGBACompositeImage* Raw3c1aToRGBAConverter::ConvertRaw3c1aToRGBA
		(composite::RawCompositeImage* rawCompositeImage)
		{
			RGBACompositeImage* rgbaImage = new RGBACompositeImage();
			rgbaImage->SetPixelSize(rawCompositeImage->GetPixelSize());

			ConvertLayer(rawCompositeImage->mainFolder, rgbaImage->mainFolder);

			return rgbaImage;
		}

		void Raw3c1aToRGBAConverter::ConvertLayer(
			composite::Layer<composite::RawImage>* from, composite::Layer<cv::Mat4d>* to)
		{
			if (from->IsFolder() != to->IsFolder())
				throw "Layer types don't match!";
			else if (from->IsFolder())
				ConvertFolderLayer(
					(composite::FolderLayer<composite::RawImage>*)from, 
					(composite::FolderLayer<cv::Mat4d>*)to);
			else
				ConvertImageLayer(
					(composite::ImageLayer<composite::RawImage>*)from,
					(composite::ImageLayer<cv::Mat4d>*)to);
		}

		void Raw3c1aToRGBAConverter::ConvertFolderLayer(
			composite::FolderLayer<composite::RawImage>* from,
			composite::FolderLayer<cv::Mat4d>* to)
		{
			for (auto sourceLayer = from->layers->begin();
				sourceLayer != from->layers->end(); sourceLayer++)
			{
				composite::Layer<cv::Mat4d>* newLayer;
				if ((*sourceLayer)->IsFolder())
					newLayer = new composite::FolderLayer<cv::Mat4d>
					((*sourceLayer)->name, (*sourceLayer)->blendingMode);
				else
					newLayer = new composite::ImageLayer<cv::Mat4d>
					((*sourceLayer)->name, (*sourceLayer)->blendingMode);

				to->layers->push_back(newLayer);
				ConvertLayer((*sourceLayer), newLayer);
			}
		}

		void Raw3c1aToRGBAConverter::ConvertImageLayer(
			composite::ImageLayer<composite::RawImage>* from,
			composite::ImageLayer<cv::Mat4d>* to)
		{
			to->offset = from->offset;

			cv::Mat4d* image = ConvertRaw3c1aToRGBA(from->GetImage());

			to->SetImage(image);
		}

		cv::Mat4d* Raw3c1aToRGBAConverter::ConvertRaw3c1aToRGBA
		(composite::RawImage* rawImage)
		{
			if (!rawImage->limitedDynamicRange)
				throw "Only images with limited range can be converted!";

			cv::Mat4d* result = new cv::Mat4d(rawImage->resolutionPx.second, 
				rawImage->resolutionPx.first, CV_32FC4);

			for (int i = 0; i < result->rows; i++)
				for (int j = 0; j < result->cols; j++)
				{
					std::vector<double> colorPixel = rawImage->image[i*result->cols + j];
					double alpha = rawImage->alpha[i * result->cols + j];

					//OpenCV uses BGRA, but RawImage uses RGBA
					cv::Vec4d resultPixel = cv::Vec4d(
						(colorPixel[2] - rawImage->dynamicRange.first)/
						(rawImage->dynamicRange.second - rawImage->dynamicRange.first),
						(colorPixel[1] - rawImage->dynamicRange.first) /
						(rawImage->dynamicRange.second - rawImage->dynamicRange.first),
						(colorPixel[0] - rawImage->dynamicRange.first) /
						(rawImage->dynamicRange.second - rawImage->dynamicRange.first),
						alpha
					);
					if (resultPixel[0] < 0) resultPixel[0] = 0;
					if (resultPixel[0] > 1) resultPixel[0] = 1;
					if (resultPixel[1] < 0) resultPixel[1] = 0;
					if (resultPixel[1] > 1) resultPixel[1] = 1;
					if (resultPixel[2] < 0) resultPixel[2] = 0;
					if (resultPixel[2] > 1) resultPixel[2] = 1;

					result->at<cv::Vec4d>(j, i) = resultPixel;

					if (i == 300 && j == 300)
						int a = 5;
				}

			cv::Vec4d a = result->at<cv::Vec4d>(0, 0);
			cv::Vec4d b = result->at<cv::Vec4d>(300, 300);
			cv::Vec4d c = result->at<cv::Vec4d>(500, 500);

			return result;
		}
	}
}