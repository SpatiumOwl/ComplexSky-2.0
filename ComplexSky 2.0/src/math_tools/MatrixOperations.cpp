#include "MatrixOperations.h"
#include <utility>
#include <vector>

namespace cs
{
	namespace math_tools
	{		
		std::vector<std::vector<double>> MatrixOperations::MultiplyMatrices(
			const std::vector<std::vector<double>>* matrix1,
			const std::vector<std::vector<double>>* matrix2)
		{
			CheckIfCanBeMultiplied(matrix1, matrix2);

			std::pair<int, int> size(matrix1->size(), (*matrix2)[0].size());

			std::vector<std::vector<double>> result = 
				std::vector<std::vector<double>>(size.first, std::vector<double>(size.second, 0));

			for (int i = 0; i < size.first; i++)
				for (int j = 0; j < size.second; j++)
					for (int k = 0; k < (*matrix2).size(); k++)
						result[i][j] += (*matrix1)[i][k] * (*matrix2)[k][j];

			return result;
		}
		void MatrixOperations::CheckIfCanBeMultiplied(
			const std::vector<std::vector<double>>* matrix1,
			const std::vector<std::vector<double>>* matrix2)
		{
			if (MatrixIsEmpty(matrix1) || MatrixIsEmpty(matrix2))
				throw "One or more matrices empty!";

			if ((*matrix1)[0].size() != matrix2->size())
				throw "Matrices cannot be multiplied!";
		}

		bool MatrixOperations::MatrixIsEmpty(
			const std::vector<std::vector<double>>* matrix)
		{
			return matrix->empty() || (*matrix)[0].empty();
		}

		std::vector<std::vector<double>> MatrixOperations::TransposeMatrix(
			const std::vector<std::vector<double>>* matrix)
		{
			std::pair<int, int> size((*matrix)[0].size(), matrix->size());

			std::vector<std::vector<double>> result(size.first, std::vector<double>(size.second, 0));

			for (int i = 0; i < size.first; i++)
				for (int j = 0; j < size.second; j++)
					result[i][j] = (*matrix)[j][i];

			return result;
		}

		std::vector<std::vector<double>> MatrixOperations::GetOnesMatrix(int n)
		{
			std::vector<std::vector<double>> result = 
				std::vector<std::vector<double>>(n, std::vector<double>(n, 0));

			for (int i = 0; i < n; i++)
				result[i][i] = 1.0;

			return result;
		}
	}
}