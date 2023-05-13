#pragma once
#include <utility>
#include <vector>
#include "color_spectrum/ColorSpectrum.h"
#include "composite/RawCompositeImage.h"
#include "sky/Sky.h"

namespace cs
{
	namespace composite_camera
	{
		class CompositeCamera
		{
		public:
			color_spectrum::ColorSpectrum* targetSpectrum;
			std::pair<unsigned int, unsigned int> resolutionPx;
			bool limitDynamicRange;
			std::pair<double, double> dynamicRange;

		private:
			CompositeCamera(color_spectrum::ColorSpectrum* _targetSpectrum,
				std::pair<unsigned int, unsigned int> _resolutionPx,
				std::pair<double, double> _dynamicRange, bool _limitDynamicRange);

			void CaptureLights(sky::Sky* sky, std::pair<double, double> cameraPos,
				std::pair<double, double> cameraSize, composite::RawCompositeImage* image);

			void CapturePointLight(sky::Sky* sky, std::pair<double, double> cameraPos,
				std::pair<double, double> cameraSize, composite::RawCompositeImage* image, 
				sky::PointLightSource* lightSource, int lightCounter);

			void CapturePurePointLightImage(composite::RawImage& lightImage, 
				std::pair<double, double>& cameraSize, std::pair<double, double>& cameraPos, 
				sky::PointLightSource* lightSource, composite::RawCompositeImage* image);

			double GetDistanceSquared(std::pair<double, double> pos1, std::pair<double, double> pos2);

			void CaptureAtmosphereAmbience(sky::Sky* sky, std::pair<double, double> cameraPos,
				std::pair<double, double> cameraSize, composite::RawCompositeImage* image);

			void CapturePureAtmosphereAmbience(sky::Sky* sky, composite::RawImage* ambienceImage);

			std::vector<double> CalculateAtmosphereAmbientColor(sky::Sky* sky);

			std::vector<double> CalculateNormalizedAmbientColor(sky::Sky* sky);

			void CreateShadedCloudMap(sky::Sky* sky, std::pair<double, double> cameraPos,
				std::pair<double, double> cameraSize, composite::RawCompositeImage* image);
			
			void CreateCloudThicknessMap(
				sky::Clouds* clouds, std::vector<double>* cloudMap,
				std::pair<double, double> cameraPos, std::pair<double, double> cameraSize);

			void CreatePlainCloudImage(sky::Sky* sky,
				composite::RawImage* plainCloudImage, std::vector<double>* cloudThicknessMap);

			std::vector<double> CalculateCloudAmbientColor(sky::Sky* sky);

			void CreateCloudHighlightsImage(sky::Sky* sky,
				composite::RawImage* cloudhighlightsImage, std::vector<double>* cloudThicknessMap,
				std::pair<double, double> cameraPos, std::pair<double, double> cameraSize);

			std::vector<double> CalculateCloudHighlightColor(sky::Sky* sky,
				std::pair<double, double> cameraPos, std::pair<double, double> cameraSize,
				int col, int row, std::vector<double>* cloudThicknessMap);

			void AddCloudHighlightFromPointLight(sky::Sky* sky, sky::PointLightSource* lightSource,
				std::pair<double, double> cameraPos, std::pair<double, double> cameraSize,
				int col, int row, std::vector<double>* cloudThicknessMap, std::vector<double>* highlightColor);

			double FindDifference(sky::Sky* sky, sky::PointLightSource* lightSource,
				std::pair<double, double> cameraPos, std::pair<double, double> cameraSize,
				int col, int row, std::vector<double>* cloudThicknessMap);


		public:
			static CompositeCamera CreateWithLimitedDynamicRange(
				color_spectrum::ColorSpectrum* targetSpectrum,
				std::pair<unsigned int, unsigned int> resolutionPx, 
				std::pair<double, double> dynamicRange);

			static CompositeCamera CreateWithUnlimitedDynamicRange(
				color_spectrum::ColorSpectrum* targetSpectrum,
				std::pair<unsigned int, unsigned int> resolutionPx);

			composite::RawCompositeImage Capture(sky::Sky* sky,
				std::pair<double, double> cameraPos,
				std::pair<double, double> cameraSize);

		};
	}
}