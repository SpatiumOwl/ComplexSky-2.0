#pragma once
#include <string>
#include "composite_processing/RGBACompositeImage.h"

#include "vendor/psd_sdk-master/Psd.h"
#include "vendor/psd_sdk-master/PsdPlatform.h"
#include "vendor/psd_sdk-master/PsdMallocAllocator.h"
#include "vendor/psd_sdk-master/PsdNativeFile.h"
#include "vendor/psd_sdk-master/PsdDocument.h"
#include "vendor/psd_sdk-master/PsdColorMode.h"
#include "vendor/psd_sdk-master/PsdLayer.h"
#include "vendor/psd_sdk-master/PsdChannel.h"
#include "vendor/psd_sdk-master/PsdChannelType.h"
#include "vendor/psd_sdk-master/PsdLayerMask.h"
#include "vendor/psd_sdk-master/PsdVectorMask.h"
#include "vendor/psd_sdk-master/PsdLayerMaskSection.h"
#include "vendor/psd_sdk-master/PsdImageDataSection.h"
#include "vendor/psd_sdk-master/PsdImageResourcesSection.h"
#include "vendor/psd_sdk-master/PsdParseDocument.h"
#include "vendor/psd_sdk-master/PsdParseLayerMaskSection.h"
#include "vendor/psd_sdk-master/PsdParseImageDataSection.h"
#include "vendor/psd_sdk-master/PsdParseImageResourcesSection.h"
#include "vendor/psd_sdk-master/PsdLayerCanvasCopy.h"
#include "vendor/psd_sdk-master/PsdInterleave.h"
#include "vendor/psd_sdk-master/PsdPlanarImage.h"
#include "vendor/psd_sdk-master/PsdExport.h"
#include "vendor/psd_sdk-master/PsdExportDocument.h"
PSD_PUSH_WARNING_LEVEL(0)
// disable annoying warning caused by xlocale(337): warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#pragma warning(disable:4530)
#include <sstream>
PSD_POP_WARNING_LEVEL

namespace cs
{
	namespace exporters
	{
		class PSDExporter
		{
		public:
			static int ExportPSD(
				composite_processing::RGBACompositeImage* compositeImage,
				std::string path);
		private:
			static void WriteLayerToPSD(psd::ExportDocument* document, psd::MallocAllocator& allocator,
				std::vector<cs::composite::Layer<cv::Mat4d>*>::iterator& layer, psd::NativeFile& file);

			static void ObtainImageLayer(std::vector<cs::composite::Layer<cv::Mat4d>*>::iterator& layer,
				cs::composite::ImageLayer<cv::Mat4d>& image);

			static void WriteFromImageLayerToExportLayer(cs::composite::ImageLayer<cv::Mat4d>& image,
				psd::ExportDocument* document, psd::MallocAllocator& allocator, unsigned int exportLayer);

			static std::vector<uint8_t*> SplitImageIntoChannels(cs::composite::ImageLayer<cv::Mat4d>& image);

			static void UploadColorChannelsToExportLayer(psd::ExportDocument* document,
				psd::MallocAllocator& allocator, unsigned int exportLayer,
				cs::composite::ImageLayer<cv::Mat4d>& image, std::vector<uint8_t*>& colorChannels);	

			static void DeleteColorChannels(std::vector<uint8_t*>& colorChannels);
		};
	}
}