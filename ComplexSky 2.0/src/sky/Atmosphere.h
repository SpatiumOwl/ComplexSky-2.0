#pragma once
#include <vector>

namespace cs
{
    namespace sky {
        class Atmosphere
        {
        private:
            double regularLightIntensity = 0.3;
            double ambientLightIntensity = 1;
            double lightFilterIntensity = 0.2;
        public:

            //Sets multiplier to the intensity of the emitting light objects
            inline void SetRegularLightIntensity(double value)
            {
                if (value >= 0) regularLightIntensity = value;
            }
            inline double GetRegularLightIntensity() { return regularLightIntensity; }
            //Sets ambient brightness of the sky, where 1 - pure color with value = 0. More than 1 - camera overexposure
            inline void SetAmbientLightIntensity(double value)
            {
                if (value >= 0) ambientLightIntensity = value;
            }
            inline double GetAmbientLightIntensity() { return ambientLightIntensity; }
            //Filter affects orange shift for point light objects and blue shift for the sky
            inline void SetLightFilterIntensity(double value)
            {
                if (value >= 0 && value <= 1)
                    lightFilterIntensity = value;
            }
            inline double GetLightFilterIntensity() { return lightFilterIntensity; }
        };
    }
}