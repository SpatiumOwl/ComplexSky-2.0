#pragma once
#include<vector>
#include "color_spectrum/ColorSpectrum.h"

namespace cs
{
	namespace color_spectrum
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