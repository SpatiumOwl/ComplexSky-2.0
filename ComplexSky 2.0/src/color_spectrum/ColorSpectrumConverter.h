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

			ColorSpectrumConverter(const ColorSpectrum* source, const ColorSpectrum* target);
			inline ~ColorSpectrumConverter() {}
			std::vector<double> Convert(const std::vector<double>* source);
		};
	}
}