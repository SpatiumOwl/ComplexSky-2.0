#pragma once
#include<vector>
#include "spectrum_db/ColorSpectrum.h"

namespace cs
{
	namespace spectrum_db
	{
		class ColorSpectrumConverter
		{
		private:

			std::vector<std::vector<double>> function;

		public:

			ColorSpectrumConverter(ColorSpectrum* source, ColorSpectrum* target);
			inline ~ColorSpectrumConverter() {}
			std::vector<double> Convert(std::vector<double> source);
		};
	}
}