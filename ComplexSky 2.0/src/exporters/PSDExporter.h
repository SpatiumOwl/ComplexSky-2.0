#pragma once
#include <string>
#include "composite_processing/RGBACompositeImage.h"

namespace cs
{
	namespace exporters
	{
		static class PSDExporter
		{
		public:
			static int ExportPSD(
				composite_processing::RGBACompositeImage compositeImage,
				std::string path);
		};
	}
}