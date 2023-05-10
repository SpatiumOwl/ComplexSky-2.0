#include "RawCompositeImage.h"

namespace cs
{
	namespace composite
	{
		RawCompositeImage::RawCompositeImage()
		{
			mainFolder = FolderLayer<RawImage>("Main Folder", BlendingMode::CS_SCREEN);
			resolutionPx = std::pair<unsigned int, unsigned int>(0, 0);
			limitedDynamicRange = false;
			dynamicRange = std::pair<double, double>(0, 0);
		}
	}
}