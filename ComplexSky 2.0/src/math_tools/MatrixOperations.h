#pragma once
#include <vector>

namespace cs
{
	namespace math_tools
	{
		static class MatrixOperations
		{
		public:
			static std::vector<std::vector<double>> MultiplyMatrices(
				const std::vector<std::vector<double>>* matrix1,
				const std::vector<std::vector<double>>* matrix2);
			static std::vector<std::vector<double>> TransposeMatrix(
				const std::vector<std::vector<double>>* matrix);
		private:
			static void CheckIfCanBeMultiplied(
				const std::vector<std::vector<double>>* matrix1,
				const std::vector<std::vector<double>>* matrix2);
			static bool MatrixIsEmpty(const std::vector<std::vector<double>>* matrix);
			static void InitializeMatrix(
				std::vector<std::vector<double>>* matrix,
				std::pair<int, int> size);
		};
	}
}