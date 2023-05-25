#pragma once
#include <vector>
#include <utility>
#include "composite/RawImage.h"

namespace cs
{
	namespace sky
	{
		enum class LightSourceType
		{
			CS_POINT,
			CS_AMBIENT,
			CS_TEXTURE
		};

		struct LightSource
		{
			LightSourceType type;
			color_spectrum::ColorSpectrum* colorSpectrum = nullptr;
		};
		struct ColoredLightSource : public LightSource
		{
			std::vector<double> normalizedColor;
			double intensity = 0;
		};
		struct PointLightSource : public ColoredLightSource
		{
			std::pair<double, double> pos;
			inline PointLightSource() {type = LightSourceType::CS_POINT;}
		};
		struct AmbientLightSource : public ColoredLightSource
		{ 
			inline AmbientLightSource() { type = LightSourceType::CS_AMBIENT; }
		};
		struct TextureLightSource : public LightSource
		{
			composite::RawImage* texture;
			std::pair<double, double> pos;
			std::pair<double, double> size;
			inline TextureLightSource() { type = LightSourceType::CS_TEXTURE; }
		};
	}
}