#pragma once
#include "sky/Clouds.h"
#include "sky/Atmosphere.h"
#include "sky/LightSources.h"
#include <vector>

namespace cs
{
	namespace sky
	{
		struct Sky
		{
			Clouds clouds;
			Atmosphere atmosphere;
			std::vector<LightSource*> lightSources;
		};
	}
}
