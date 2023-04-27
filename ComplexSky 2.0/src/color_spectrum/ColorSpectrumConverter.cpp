#include "ColorSpectrumConverter.h"
#include "math_tools/MatrixOperations.h"

namespace cs
{
	namespace color_spectrum
	{
		std::vector<double> ColorSpectrumConverter::Convert(const std::vector<double>* source)
		{
			std::vector<std::vector<double>> inputMatrix = { *source };

			std::vector<std::vector<double>> outputMatrix =
				math_tools::MatrixOperations::MultiplyMatrices(&inputMatrix, &function);

			return math_tools::MatrixOperations::TransposeMatrix(&outputMatrix)[0];
		}
	}
}