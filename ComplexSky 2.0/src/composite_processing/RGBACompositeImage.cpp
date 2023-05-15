#include "RGBACompositeImage.h"

namespace cs
{
	namespace composite_processing
	{
		RGBACompositeImage::RGBACompositeImage()
		{
			mainFolder = new composite::FolderLayer<cv::Mat4d>("Main Folder", composite::BlendingMode::CS_SCREEN);
			resolutionPx = std::pair<unsigned int, unsigned int>(0, 0);
		}
	}
}