#pragma once
#include "RGBACompositeImage.h"
#include "composite/RawCompositeImage.h"

namespace cs
{
	namespace composite_processing
	{
		static class Raw3c1aToRGBAConverter
		{
		public:
			static RGBACompositeImage ConvertRaw3c1aToRGBA
			(composite::RawCompositeImage rawCompositeImage);
		};
	}
}