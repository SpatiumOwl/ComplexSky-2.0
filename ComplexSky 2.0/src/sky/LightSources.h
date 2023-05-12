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
		};
		struct ColoredLightSource : LightSource
		{
			color_spectrum::ColorSpectrum* colorSpectrum;
			std::vector<double> color;
			double intensity;
		};
		struct PointLightSource : ColoredLightSource
		{
			std::pair<double, double> pos;
			LightSourceType type = LightSourceType::CS_POINT;
		};
		struct AmbientLightSource : ColoredLightSource
		{
			LightSourceType type = LightSourceType::CS_AMBIENT;
		};
		struct TextureLightSource : LightSource
		{
			composite::RawImage texture;
			std::pair<double, double> pos;
			std::pair<double, double> size;
			LightSourceType type = LightSourceType::CS_TEXTURE;
		};
	}
}