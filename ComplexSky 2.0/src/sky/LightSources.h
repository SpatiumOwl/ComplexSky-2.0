#pragma once
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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
			std::vector<double> color;
			double intensity;
		};
		struct PointLightSource : ColoredLightSource
		{
			cv::Vec2d pos;
			LightSourceType type = LightSourceType::CS_POINT;
		};
		struct AmbientLightSource : ColoredLightSource
		{
			LightSourceType type = LightSourceType::CS_AMBIENT;
		};
		struct TextureLightSource : LightSource
		{
			composite::RawImage texture;
			cv::Vec2d pos;
			cv::Vec2d size;
			LightSourceType type = LightSourceType::CS_TEXTURE;
		};
	}
}