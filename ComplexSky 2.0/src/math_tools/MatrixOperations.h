#pragma once
#include <vector>

namespace cs
{
	namespace math_tools
	{
		class MatrixOperations
		{
		public:
			static std::vector<std::vector<double>> MultiplyMatrices(
				const std::vector<std::vector<double>>* matrix1,
				const std::vector<std::vector<double>>* matrix2);
			static std::vector<std::vector<double>> TransposeMatrix(
				const std::vector<std::vector<double>>* matrix);
			static std::vector<std::vector<double>> GetOnesMatrix(int n);
		private:
			static void CheckIfCanBeMultiplied(
				const std::vector<std::vector<double>>* matrix1,
				const std::vector<std::vector<double>>* matrix2);
			static bool MatrixIsEmpty(const std::vector<std::vector<double>>* matrix);
		};
	}
}