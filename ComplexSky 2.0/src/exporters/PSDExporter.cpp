#include "PSDExporter.h"
#include "RGBAComposer.h"

namespace cs
{
	namespace exporters
	{
		int PSDExporter::ExportPSD(
			composite_processing::RGBACompositeImage* compositeImage,
			std::string path)
		{
			const std::wstring dstPath(path.begin(), path.end());

			psd::MallocAllocator allocator;
			psd::NativeFile file(&allocator);

			if (!file.OpenWrite(dstPath.c_str()))
			{
				OutputDebugStringA("Cannot open file.\n");
				return 1;
			}

			psd::ExportDocument* document = psd::CreateExportDocument(&allocator, 
				compositeImage->GetPixelSize().first, compositeImage->GetPixelSize().second, 
				8u, psd::exportColorMode::RGB);

			for (auto layer = compositeImage->mainFolder->layers->begin(); 
				layer != compositeImage->mainFolder->layers->end(); layer++)
				WriteLayerToPSD(document, allocator, layer, file);

			psd::DestroyExportDocument(document, &allocator);
			file.Close();

			return 0;
		}

		void PSDExporter::WriteLayerToPSD(psd::ExportDocument* document, psd::MallocAllocator& allocator,
			std::vector<cs::composite::Layer<cv::Mat4d>*>::iterator& layer, 
			psd::NativeFile& file)
		{
			unsigned int exportLayer = psd::AddLayer(document, &allocator, (*layer)->name.c_str());
			composite::ImageLayer<cv::Mat4d> image;
			ObtainImageLayer(layer, image);

			WriteFromImageLayerToExportLayer(image, document, allocator, exportLayer);

			psd::WriteDocument(document, &allocator, &file);
		}
		
		void PSDExporter::ObtainImageLayer(std::vector<cs::composite::Layer<cv::Mat4d>*>::iterator& layer,
			cs::composite::ImageLayer<cv::Mat4d>& image)
		{
			if (!(*layer)->IsFolder())
				image = *static_cast<composite::ImageLayer<cv::Mat4d>*>(*layer);
			else
			{
				image.blendingMode = (*layer)->blendingMode;
				image.name = (*layer)->name;
				composite::FolderLayer<cv::Mat4d>* folder = static_cast<composite::FolderLayer<cv::Mat4d>*>(*layer);
				for (auto curLayer = folder->layers->begin(); curLayer != folder->layers->end(); curLayer++)
					RGBAComposer::ComposeLayer(&image, *curLayer);
			}
		}

		void PSDExporter::WriteFromImageLayerToExportLayer(cs::composite::ImageLayer<cv::Mat4d>& image,
			psd::ExportDocument* document, psd::MallocAllocator& allocator, 
			unsigned int exportLayer)
		{			
			std::vector<uint8_t*> colorChannels = SplitImageIntoChannels(image);
			
			UploadColorChannelsToExportLayer(document, allocator, exportLayer, image, colorChannels);

			DeleteColorChannels(colorChannels);
		}
		
		std::vector<uint8_t*> PSDExporter::SplitImageIntoChannels(cs::composite::ImageLayer<cv::Mat4d>& image)
		{
			uint8_t* blueChannel = new uint8_t[image.GetImage()->rows * image.GetImage()->cols];
			uint8_t* greenChannel = new uint8_t[image.GetImage()->rows * image.GetImage()->cols];
			uint8_t* redChannel = new uint8_t[image.GetImage()->rows * image.GetImage()->cols];
			uint8_t* alphaChannel = new uint8_t[image.GetImage()->rows * image.GetImage()->cols];
			int i = 0;
			for (auto pixel = image.GetImage()->begin(); pixel != image.GetImage()->end(); pixel++)
			{
				//Put data into channels
				blueChannel[i] = (uint8_t)((*pixel)[0] * 255);
				greenChannel[i] = (uint8_t)((*pixel)[1] * 255);
				redChannel[i] = (uint8_t)((*pixel)[2] * 255);
				alphaChannel[i] = (uint8_t)((*pixel)[3] * 255);
				//Increment
				i++;
			}
			return std::vector<uint8_t*> { blueChannel, greenChannel, redChannel, alphaChannel };
		}

		void PSDExporter::UploadColorChannelsToExportLayer(psd::ExportDocument* document, psd::MallocAllocator& allocator,
			unsigned int exportLayer, cs::composite::ImageLayer<cv::Mat4d>& image, 
			std::vector<uint8_t*>& colorChannels)
		{
			psd::UpdateLayer(document, &allocator, exportLayer, psd::exportChannel::RED,
				image.offset.second, image.offset.first, image.offset.second + image.GetImage()->cols,
				image.offset.first + image.GetImage()->rows, colorChannels[2], psd::compressionType::RAW);
			psd::UpdateLayer(document, &allocator, exportLayer, psd::exportChannel::GREEN,
				image.offset.second, image.offset.first, image.offset.second + image.GetImage()->cols,
				image.offset.first + image.GetImage()->rows, colorChannels[1], psd::compressionType::RAW);
			psd::UpdateLayer(document, &allocator, exportLayer, psd::exportChannel::BLUE,
				image.offset.second, image.offset.first, image.offset.second + image.GetImage()->cols,
				image.offset.first + image.GetImage()->rows, colorChannels[0], psd::compressionType::RAW);
			psd::UpdateLayer(document, &allocator, exportLayer, psd::exportChannel::ALPHA,
				image.offset.second, image.offset.first, image.offset.second + image.GetImage()->cols,
				image.offset.first + image.GetImage()->rows, colorChannels[3], psd::compressionType::RAW);
		}
		
		void PSDExporter::DeleteColorChannels(std::vector<uint8_t*>& colorChannels)
		{
			delete[] colorChannels[0];
			delete[] colorChannels[1];
			delete[] colorChannels[2];
			delete[] colorChannels[3];
		}
	}
}