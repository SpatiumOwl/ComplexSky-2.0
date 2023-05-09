#include <vector>

namespace cs
{
	namespace math_tools
	{
		static class Integrator
		{
		public:
			static std::vector<double> FindIntersection(
				std::vector<double> func1, double distance1, 
				std::vector<double> func2, double distance2);

			static double FindArea(
				std::vector<double> func, double distance);

		private:
			static std::pair<double, std::pair<std::vector<double>, std::vector<double>>> BringToScale(
				std::vector<double> func1, double distance1,
				std::vector<double> func2, double distance2);

			static std::pair<double, std::pair<std::vector<double>, std::vector<double>>> BringToSameDistance(
				std::vector<double> func1, double distance1,
				std::vector<double> func2, double distance2);

			static std::pair<std::vector<double>, std::vector<double>> BringToSameSize(
				std::vector<double> func1, std::vector<double> func2);

			static std::vector<double> ConvertToNewDistance(
				std::vector<double> func, double oldDistance,
				double newDistance);
		};
	}
}


