#include "Converter.h"

namespace cs
{
	namespace rtc
	{
		composite::RawImage* Converter::ImportImage(
			std::string path, std::pair<double, double> dynamicRange)
		{
			cv::Mat rgb8Image = cv::imread(path, 0);
			composite::RawImage* image = new composite::RawImage(
				true, dynamicRange, std::pair<uint, uint>(rgb8Image.cols, rgb8Image.rows));

			for (int row = 0; row < rgb8Image.rows; row++)
				for (int col = 0; col < rgb8Image.cols; col++)
				{
					cv::Vec4w inputPixel = rgb8Image.at<cv::Vec4i>(row, col);
					image->alpha[row * rgb8Image.cols + col] = (double)inputPixel[3] / 255.0;

					//Input pixel is in BGR, need to convert it to RGB
					image->image[row * rgb8Image.cols + col] = std::vector<double>{
						(double)inputPixel[2] / 255.0,
						(double)inputPixel[1] / 255.0,
						(double)inputPixel[0] / 255.0
					};
				}

			return image;
		}
	}
}