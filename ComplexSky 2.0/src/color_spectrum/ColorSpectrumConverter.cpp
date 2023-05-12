#include "ColorSpectrumConverter.h"
#include "math_tools/MatrixOperations.h"
#include "math_tools/Integrator.h"

namespace cs
{
	namespace color_spectrum
	{
		ColorSpectrumConverter::ColorSpectrumConverter(const ColorSpectrum* source, const ColorSpectrum* target)
		{
			function = std::vector<std::vector<double>>();
			for (int i = 0; i < source->colorChannels.size(); i++)
				function.push_back(std::vector<double>(target->colorChannels.size()));

			for (int i = 0; i < source->colorChannels.size(); i++)
				for (int j = 0; j < target->colorChannels.size(); j++)
				{
					std::pair<double, std::vector<double>> intersectionSpectrum = 
						math_tools::Integrator::FindIntersection(
						source->colorChannels[i].func, source->colorChannels[i].distanceBetweenValues,
						target->colorChannels[j].func, target->colorChannels[j].distanceBetweenValues);

					double intersectionArea = math_tools::Integrator::FindArea(
						intersectionSpectrum.second, intersectionSpectrum.first);
					double targetArea = math_tools::Integrator::FindArea(
						target->colorChannels[j].func, target->colorChannels[j].distanceBetweenValues
					);

					if (targetArea == 0 || intersectionArea == 0)
						function[i][j] = 0;
					else
						function[i][j] = intersectionArea / targetArea;
				}
			
		}
		std::vector<double> ColorSpectrumConverter::ConvertRaw(const std::vector<double>* source)
		{
			std::vector<std::vector<double>> inputMatrix = { *source };

			std::vector<std::vector<double>> outputMatrix =
				math_tools::MatrixOperations::MultiplyMatrices(&inputMatrix, &function);

			return math_tools::MatrixOperations::TransposeMatrix(&outputMatrix)[0];
		}
		std::vector<double> ColorSpectrumConverter::ConvertNormalized(const std::vector<double>* source)
		{
			std::vector<double> pixel = ConvertRaw(source);

			double maxVal = -DBL_MAX; int maxInd = -1;
			for (int i = 0; i < pixel.size(); i++)
				if (pixel[i] >= maxVal)
				{
					maxVal = pixel[i];
					maxInd = i;
				}

			if (maxInd == 0)
				return pixel;
			
			for (auto val = pixel.begin(); val != pixel.end(); val++)
				*val /= maxVal;

			return pixel;
		}
	}
}