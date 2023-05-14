#include "CompositeCamera.h"
#include "math_tools/PerlinNoiseGenerator.h"
#include "color_spectrum/ColorSpectrumDB.h"
#include <iostream>

namespace cs
{
	namespace composite_camera
	{
		CompositeCamera::CompositeCamera(color_spectrum::ColorSpectrum* _targetSpectrum,
			std::pair<unsigned int, unsigned int> _resolutionPx,
			std::pair<double, double> _dynamicRange, bool _limitDynamicRange)
		{
			targetSpectrum = _targetSpectrum;
			resolutionPx = _resolutionPx;
			dynamicRange = _dynamicRange;
			limitDynamicRange = _limitDynamicRange;
		}

		CompositeCamera CompositeCamera::CreateWithLimitedDynamicRange(
			color_spectrum::ColorSpectrum* targetSpectrum,
			std::pair<unsigned int, unsigned int> resolutionPx,
			std::pair<double, double> dynamicRange)
		{
			return CompositeCamera(targetSpectrum, resolutionPx, dynamicRange, true);
		}

		CompositeCamera CompositeCamera::CreateWithUnlimitedDynamicRange(
			color_spectrum::ColorSpectrum* targetSpectrum,
			std::pair<unsigned int, unsigned int> resolutionPx)
		{
			return CompositeCamera(targetSpectrum, resolutionPx, 
				std::pair<double, double>(0, 0), false);
		}

		composite::RawCompositeImage* CompositeCamera::Capture(sky::Sky* sky,
			std::pair<double, double> cameraPos, std::pair<double, double> cameraSize)
		{
			composite::RawCompositeImage* result = new composite::RawCompositeImage();
			result->colorSpectrum = targetSpectrum;
			result->SetPixelSize(resolutionPx);
			result->limitedDynamicRange = limitDynamicRange;
			result->dynamicRange = dynamicRange;

			CaptureAtmosphereAmbience(sky, cameraPos, cameraSize, result);
			CaptureLights(sky, cameraPos, cameraSize, result);
			CreateShadedCloudMap(sky, cameraPos, cameraSize, result);

			return result;
		}

		void CompositeCamera::CaptureLights(sky::Sky* sky, std::pair<double, double> cameraPos,
			std::pair<double, double> cameraSize, composite::RawCompositeImage* image)
		{
			int lightCounter = 0;
			for (auto lightSource = sky->lightSources.begin(); 
				lightSource != sky->lightSources.end(); lightSource++)
			{
				lightCounter++;
				if (lightSource->type == sky::LightSourceType::CS_POINT)
					CapturePointLight(sky, cameraPos, cameraSize, image,
						static_cast<sky::PointLightSource*>(&(*lightSource)), lightCounter);
			}
		}

		void CompositeCamera::CapturePointLight(sky::Sky* sky, std::pair<double, double> cameraPos,
			std::pair<double, double> cameraSize,
			composite::RawCompositeImage* image, sky::PointLightSource* lightSource, int lightCounter)
		{
			std::string lightName = "Light ";
			lightName += std::to_string(lightCounter);

			composite::FolderLayer<composite::RawImage>* lightFolder =
				new composite::FolderLayer<composite::RawImage>
				(lightName, composite::BlendingMode::CS_SCREEN);

			composite::RawImage lightImage = composite::RawImage(image->colorSpectrum,
				image->limitedDynamicRange, image->dynamicRange, image->GetPixelSize());

			CapturePurePointLightImage(lightImage, cameraSize, cameraPos, lightSource, image);

			composite::ImageLayer<composite::RawImage>* lightLayer =
				new composite::ImageLayer<composite::RawImage>
				("Light image", composite::BlendingMode::CS_NORMAL);

			lightLayer->SetImage(&lightImage);
			lightFolder->layers->push_back(lightLayer);

			//Filter

			/*std::vector<double> rgbFilterColor = std::vector<double>{
				1 - sky->atmosphere.GetLightFilterIntensity(), 
				0.4 + 0.6 * (1 - sky->atmosphere.GetLightFilterIntensity()), 1.0 };


			composite::ImageLayer<composite::RawImage>* filter =
				new composite::ImageLayer<composite::RawImage>
				("Red Filter", composite::BlendingMode::CS_MULTIPLY);

			cv::Mat4d filterImage = cv::Mat::zeros(imageOperator.GetPixelSize()[0], imageOperator.GetPixelSize()[1], CV_32FC4);

			for (auto pixel = filterImage.begin(); pixel != filterImage.end(); pixel++)
				*pixel = filterColor;

			filter->SetImage(&filterImage);

			lightFolder->layers->push_back(filter);
			*/

			image->PushLayer(lightFolder);
			
		}

		void CompositeCamera::CapturePurePointLightImage(cs::composite::RawImage& lightImage, 
			std::pair<double, double>& cameraSize, std::pair<double, double>& cameraPos, 
			cs::sky::PointLightSource* lightSource, cs::composite::RawCompositeImage* image)
		{
			color_spectrum::ColorSpectrumDB* colorSpectrumDB = color_spectrum::ColorSpectrumDB::GetInstance();

			for (int col = 0; col < (int)lightImage.resolutionPx.first; col++)
				for (int row = 0; row < (int)lightImage.resolutionPx.second; row++)
				{
					std::pair<double, double> pos = std::pair<double, double>(
						(col + 1.0) / lightImage.resolutionPx.first * cameraSize.first + cameraPos.first,
						(row + 1.0) / lightImage.resolutionPx.second * cameraSize.second + cameraPos.second);

					double distanceSquared = GetDistanceSquared(pos, lightSource->pos);

					color_spectrum::ColorSpectrumConverter* converter = colorSpectrumDB->GetConverter
					(lightSource->colorSpectrum->name, targetSpectrum->name);

					std::vector<double> lightSourceColor = converter->ConvertNormalized(&(lightSource->normalizedColor));
					
					std::vector<double> colorValue = std::vector<double>(targetSpectrum->colorChannels.size(), 0);
					for (int i = 0; i < colorValue.size(); i++)
						colorValue[i] = lightSourceColor[i] * lightSource->intensity / distanceSquared;

					lightImage.image[row * image->GetPixelSize().second + col] = colorValue;
				}
		}
		
		double CompositeCamera::GetDistanceSquared(
			std::pair<double, double> pos1, std::pair<double, double> pos2)
		{
			std::pair<double, double> distanceVec = std::pair<double, double>(
				pos1.first - pos2.first,
				pos1.second - pos2.second);

			return distanceVec.first * distanceVec.first +
				distanceVec.second * distanceVec.second;
		}

		void CompositeCamera::CaptureAtmosphereAmbience(
			sky::Sky* sky, std::pair<double, double> cameraPos,
			std::pair<double, double> cameraSize, composite::RawCompositeImage* image)
		{
			//Create a layer folder and push it to the main one
			composite::FolderLayer<composite::RawImage>* folder = 
				new composite::FolderLayer<composite::RawImage>
				("Ambient Light", composite::BlendingMode::CS_SCREEN);

			composite::ImageLayer<composite::RawImage>* ambience = 
				new composite::ImageLayer<composite::RawImage>
				("Ambience", composite::BlendingMode::CS_NORMAL);
			
			composite::RawImage* ambienceImage = CapturePureAtmosphereAmbience(sky);

			if (ambienceImage != nullptr)
			{
				ambience->SetImage(ambienceImage);
				folder->layers->push_back((composite::Layer<composite::RawImage>*)(ambience));
			}

			/*
			//Blue Filter
			cv::Vec4d filterColor(1.0, 0.3 + 0.7 * (1 - skyMap.GetLightFilterIntensity()), 1 - skyMap.GetLightFilterIntensity(), 1.0);
			
			ImageLayer* filter = new ImageLayer("Blue Filter", BlendingMode::CS_MULTIPLY);
			
			cv::Mat4d filterImage = cv::Mat::zeros(imageOperator.GetPixelSize()[0], imageOperator.GetPixelSize()[1], CV_32FC4);
			for (auto pixel = filterImage.begin(); pixel != filterImage.end(); pixel++)
				*pixel = filterColor;
			filter->SetImage(&filterImage);
			folder->layers->push_back((Layer*)(filter));
			*/

			image->PushLayer((composite::Layer<composite::RawImage>*)(folder));
		}

		composite::RawImage* CompositeCamera::CapturePureAtmosphereAmbience(sky::Sky* sky)
		{
			composite::RawImage* ambienceImage = new composite::RawImage(targetSpectrum, limitDynamicRange, dynamicRange, resolutionPx);

			std::vector<double> amColor = CalculateNormalizedAmbientColor(sky);

			for (auto pixel = ambienceImage->image.begin(); pixel != ambienceImage->image.end(); pixel++)
				*pixel = amColor;

			return ambienceImage;
		}

		std::vector<double> CompositeCamera::CalculateAtmosphereAmbientColor(sky::Sky* sky)
		{
			std::vector<double> amColor = CalculateNormalizedAmbientColor(sky);

			for (int i = 0; i < amColor.size(); i++)
				amColor[i] *= sky->atmosphere.GetAmbientLightIntensity();

			return amColor;
		}
		
		std::vector<double> CompositeCamera::CalculateNormalizedAmbientColor(sky::Sky* sky)
		{
			std::vector<double> amColor = std::vector<double>(targetSpectrum->colorChannels.size());

			double totalIntensity = 0;

			color_spectrum::ColorSpectrumDB* colorSpectrumDB = color_spectrum::ColorSpectrumDB::GetInstance();

			for (auto lightSource = sky->lightSources.begin();
				lightSource != sky->lightSources.end(); lightSource++)
			{
				if (lightSource->type == sky::LightSourceType::CS_POINT)
				{
					sky::PointLightSource* pointLightSource = static_cast<sky::PointLightSource*>(&(*lightSource));
					
					color_spectrum::ColorSpectrumConverter* converter = colorSpectrumDB->GetConverter
					(pointLightSource->colorSpectrum->name, targetSpectrum->name);

					std::vector<double> pointLightColor = converter->ConvertNormalized(&(pointLightSource->normalizedColor));

					for (int i = 0; i < amColor.size(); i++)
						amColor[i] += pointLightColor[i] * pointLightSource->intensity;
					totalIntensity += pointLightSource->intensity;
				}
			}
			for (int i = 0; i < amColor.size(); i++)
				amColor[i] /= totalIntensity;

			return amColor;
		}

		void CompositeCamera::CreateShadedCloudMap(
			sky::Sky* sky, std::pair<double, double> cameraPos,
			std::pair<double, double> cameraSize, composite::RawCompositeImage* image)
		{
			std::vector<double> cloudThicknessMap;
			CreateCloudThicknessMap(&(sky->clouds), &cloudThicknessMap, cameraPos, cameraSize);

			composite::RawImage plainCloudImage = composite::RawImage(
				targetSpectrum, limitDynamicRange, dynamicRange, resolutionPx);

			CreatePlainCloudImage(sky, &plainCloudImage, &cloudThicknessMap);

			composite::RawImage highlightsImage = composite::RawImage(
				targetSpectrum, limitDynamicRange, dynamicRange, resolutionPx);


			composite::FolderLayer<composite::RawImage>* folder = 
				new composite::FolderLayer<composite::RawImage>
				("Clouds", composite::BlendingMode::CS_NORMAL);

			composite::ImageLayer<composite::RawImage>* plainClouds = 
				new composite::ImageLayer<composite::RawImage>
				("Plain Clouds", composite::BlendingMode::CS_NORMAL);

			composite::ImageLayer<composite::RawImage>* highlights = 
				new composite::ImageLayer<composite::RawImage>
				("Highlights", composite::BlendingMode::CS_SCREEN);

			plainClouds->SetImage(&plainCloudImage);
			highlights->SetImage(&highlightsImage);
			folder->layers->push_back(plainClouds);
			folder->layers->push_back(highlights);

			image->PushLayer(folder);
		}

		void CompositeCamera::CreateCloudThicknessMap(
			sky::Clouds* clouds, std::vector<double>* cloudMap,
			std::pair<double, double> cameraPos, std::pair<double, double> cameraSize)
		{
			cloudMap = new std::vector<double>(resolutionPx.first * resolutionPx.second, 0);
			for (int col = 0; col < (int)resolutionPx.first; col++)
				for (int row = 0; row < (int)resolutionPx.second; row++)
				{
					//Convert to units
					double x = (col / (resolutionPx.first - 1.0) * cameraSize.first + cameraPos.first) / clouds->scale;
					double y = (row / (resolutionPx.second - 1.0) * cameraSize.second + cameraPos.second) / clouds->scale;
					//Generate cloud
					double z = math_tools::PerlinNoiseGenerator::SquareNoise2D(
						x, y, clouds->GetPersistence(), clouds->GetNumberOfOctaves(), clouds->seed) / 2 + 0.5;
					//Cutoff
					z = (z - clouds->GetCutoff()) / (1 - clouds->GetCutoff());
					if (z < 0)
						z = 0;

					//Change density
					z *= clouds->GetCloudDensity();

					if (z > 1)
						z = 1;
					//Write to cloud

					(*cloudMap)[row * resolutionPx.first + col] = z;
				}
		}

		void CompositeCamera::CreatePlainCloudImage(sky::Sky* sky, 
			composite::RawImage* plainCloudImage, std::vector<double>* cloudThicknessMap)
		{
			std::vector<double> ambientColor = CalculateCloudAmbientColor(sky);

			for (int col = 0; col < (int)resolutionPx.first; col++)
				for (int row = 0; row < (int)resolutionPx.second; row++)
					plainCloudImage->image[row * resolutionPx.second + col] = ambientColor;

			plainCloudImage->alpha = *cloudThicknessMap;
		}

		std::vector<double> CompositeCamera::CalculateCloudAmbientColor(sky::Sky* sky)
		{
			std::vector<double> amColor = CalculateNormalizedAmbientColor(sky);

			for (int i = 0; i < amColor.size(); i++)
				amColor[i] *= sky->clouds.GetAmbientBrightness();

			return amColor;
		}

		void CompositeCamera::CreateCloudHighlightsImage(sky::Sky* sky,
			composite::RawImage* cloudhighlightsImage, std::vector<double>* cloudThicknessMap,
			std::pair<double, double> cameraPos, std::pair<double, double> cameraSize)
		{
			for (int col = 0; col < (int)resolutionPx.first; col++)
				for (int row = 0; row < (int)resolutionPx.second; row++)
					cloudhighlightsImage->image[row * resolutionPx.second + col] = 
						CalculateCloudHighlightColor(
							sky, cameraPos, cameraSize, col, row, cloudThicknessMap);

			//May include alpha channel for highlights
		}

		std::vector<double> CompositeCamera::CalculateCloudHighlightColor(sky::Sky* sky,
			std::pair<double, double> cameraPos, std::pair<double, double> cameraSize, 
			int col, int row, std::vector<double>* cloudThicknessMap)
		{
			std::vector<double> highlightColor = std::vector<double>(targetSpectrum->colorChannels.size(), 0);

			for (auto lightSource = sky->lightSources.begin(); lightSource != sky->lightSources.end(); lightSource++)
				if (lightSource->type == sky::LightSourceType::CS_POINT)
					AddCloudHighlightFromPointLight(sky, static_cast<sky::PointLightSource*>(&(*lightSource)),
						cameraPos, cameraSize, col, row, cloudThicknessMap, &highlightColor);

			return highlightColor;
		}

		void CompositeCamera::AddCloudHighlightFromPointLight(sky::Sky* sky, sky::PointLightSource* lightSource,
			std::pair<double, double> cameraPos, std::pair<double, double> cameraSize,
			int col, int row, std::vector<double>* cloudThicknessMap, std::vector<double>* highlightColor)
		{
			color_spectrum::ColorSpectrumDB* colorSpectrumDB = color_spectrum::ColorSpectrumDB::GetInstance();
			color_spectrum::ColorSpectrumConverter* converter = colorSpectrumDB->GetConverter
			(lightSource->colorSpectrum->name, targetSpectrum->name);

			std::vector<double> pointLightColor =
				converter->ConvertNormalized(&(lightSource->normalizedColor));

			double difference = FindDifference(sky, lightSource, cameraPos, cameraSize, col, row, cloudThicknessMap);

			if (difference > 0)
				for (int i = 0; i < highlightColor->size(); i++)
					(*highlightColor)[i] += pointLightColor[i] * lightSource->intensity * 
					difference * sky->clouds.GetHighlightBrightness();
		}

		double CompositeCamera::FindDifference(sky::Sky* sky, sky::PointLightSource* lightSource,
			std::pair<double, double> cameraPos, std::pair<double, double> cameraSize,
			int col, int row, std::vector<double>* cloudThicknessMap)
		{
			std::pair<double, double> currentPointPosUnits = std::pair<double, double>(
				col / (resolutionPx.first - 1.0) * cameraSize.first + cameraPos.first,
				row / (resolutionPx.second - 1.0) * cameraSize.second + cameraPos.second);

			std::pair<double, double> curPointToLightSourceVecUnits = std::pair<double, double>(
				currentPointPosUnits.first - lightSource->pos.first,
				currentPointPosUnits.second - lightSource->pos.second);

			double max;
			if (std::abs(curPointToLightSourceVecUnits.first) >= std::abs(curPointToLightSourceVecUnits.second))
				max = std::abs(curPointToLightSourceVecUnits.first);
			else
				max = std::abs(curPointToLightSourceVecUnits.second);

			curPointToLightSourceVecUnits = std::pair<double, double>(
				curPointToLightSourceVecUnits.first / max * sky->clouds.GetCloudShadingSmoothness(),
				curPointToLightSourceVecUnits.second / max * sky->clouds.GetCloudShadingSmoothness());

			std::pair<double, double> newPosUnits = std::pair<double, double>(
				currentPointPosUnits.first + curPointToLightSourceVecUnits.first * 2,
				currentPointPosUnits.second + curPointToLightSourceVecUnits.second * 2);

			std::pair<int, int> newPos = std::pair<int, int>(
				(int)std::round((newPosUnits.first - cameraPos.first) /
					cameraSize.first * (resolutionPx.first - 1.0)),
				(int)std::round((newPosUnits.second - cameraPos.second) /
					cameraSize.second * (resolutionPx.second - 1.0)));

			if (newPos.first < 0) newPos.first = 0;
			if (newPos.first >= (int)resolutionPx.first) newPos.first = resolutionPx.first - 1;
			if (newPos.second < 0) newPos.second = 0;
			if (newPos.second >= (int)resolutionPx.second) newPos.second = resolutionPx.second - 1;

			double difference = (*cloudThicknessMap)[newPos.second * resolutionPx.second + newPos.first] -
				(*cloudThicknessMap)[row, col];

			return difference;
		}
	}
}