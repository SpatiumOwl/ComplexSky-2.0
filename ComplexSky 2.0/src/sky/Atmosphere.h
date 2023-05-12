#pragma once
#include <vector>

namespace cs
{
    namespace sky {
        class Atmosphere
        {
        private:
            double ambientLightIntensity = 1;
            double lightFilterIntensity = 0.2;
        public:
            //Sets ambient brightness of the sky
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