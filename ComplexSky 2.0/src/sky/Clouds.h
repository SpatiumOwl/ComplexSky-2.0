#pragma once

namespace cs
{
    namespace sky {
        class Clouds
        {
        private:
            double cloudDensity = 1;
            double cutoff = 0.5;
            double persistence = 0.6;
            unsigned int numberOfOctaves = 6;
            double ambientBrightness = 0.5;
            double highlightBrightness = 20;
            double cloudShadingSmoothness = 0.15;
        public:
            double seed = 270;
            double scale = 1;

            //Properties
            //Cut thinner part of the cloud
            inline void SetCutoff(double value)
            {
                if (value >= 0 && value <= 1) cutoff = value;
            }
            inline double GetCutoff() { return cutoff; }
            //Radius in pixel values
            inline void SetCloudShadingSmoothness(double value)
            {
                if (value > 0) cloudShadingSmoothness = value;
            }
            inline double GetCloudShadingSmoothness() { return cloudShadingSmoothness; }
            //Multiplies the opacity value of the clouds
            inline void SetCloudDensity(double value)
            {
                if (value >= 0) cloudDensity = value;
            }
            inline double GetCloudDensity() { return cloudDensity; }
            //Closer to 0 - bigger frequencies overtake, 1 - all frequencies are equal
            inline void SetPersistence(double value)
            {
                if (value > 0 && value <= 1) persistence = value;
            }
            inline double GetPersistence() { return persistence; }
            //More octaves - more smaller frequencies
            inline void SetNumberOfOctaves(unsigned int value)
            {
                if (value > 0) numberOfOctaves = value;
            }
            inline unsigned int GetNumberOfOctaves() { return numberOfOctaves; }
            //Sets ambient brightness of the clouds, where 1 - pure color with value = 0. More than 1 - camera overexposure
            inline void SetAmbientBrightness(double value)
            {
                if (value >= 0) ambientBrightness = value;
            }
            inline double GetAmbientBrightness() { return ambientBrightness; }
            //Sets the brightness of the highlights
            inline void SetHighlightBrightness(double value)
            {
                if (value >= 0) highlightBrightness = value;
            }
            inline double GetHighlightBrightness() { return highlightBrightness; }
        };
    }
}