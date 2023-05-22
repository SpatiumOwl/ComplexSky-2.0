// ComplexSky 2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "math_tools/MatrixOperations.h"
#include "sky/Sky.h"
#include "color_spectrum/ColorSpectrumDB.h"
#include "composite_camera/CompositeCamera.h"
#include "composite_processing/Raw3c1aToRGBAConverter.h"
#include "exporters/RGBAComposer.h"

using namespace cs;
int main()
{
    color_spectrum::ColorSpectrumDB* db =
        color_spectrum::ColorSpectrumDB::GetInstance();

    color_spectrum::ColorSpectrum rgb;
    rgb.name = "RGB";

    color_spectrum::ColorSpectrumChannel red;
    red.distanceBetweenValues = 25;
    red.func = std::vector<double>{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 8, 6, 6,
        6, 10, 1, 30,
        75, 50, 30, 3,
        0 };
    rgb.colorChannels.push_back(red);
        
    color_spectrum::ColorSpectrumChannel green;
    green.distanceBetweenValues = 25;
    green.func = std::vector<double>{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 9, 15, 38,
        50, 90, 100, 50,
        10, 3, 1, 0 };
    rgb.colorChannels.push_back(green);

    color_spectrum::ColorSpectrumChannel blue;
    blue.distanceBetweenValues = 25;
    blue.func = std::vector<double>{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 30, 85, 95,
        50, 9, 1, 0};
    rgb.colorChannels.push_back(blue);

    db->AddSpectrum(&rgb);

    color_spectrum::ColorSpectrum hubble;
    hubble.name = "Hubble";

    color_spectrum::ColorSpectrumChannel s2;
    s2.distanceBetweenValues = 12.5;
    s2.func = std::vector<double>{
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2, 90, 2,
        0
    };
    hubble.colorChannels.push_back(s2);

    color_spectrum::ColorSpectrumChannel ha;
    ha.distanceBetweenValues = 12.5;
    ha.func = std::vector<double>{
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 90, 2, 0,
    };
    hubble.colorChannels.push_back(ha);

    color_spectrum::ColorSpectrumChannel o3;
    o3.distanceBetweenValues = 12.5;
    o3.func = std::vector<double>{
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2,
        90, 2, 0
    };
    hubble.colorChannels.push_back(o3);

    db->AddSpectrum(&hubble);
        
    sky::Sky sky;
    sky.clouds.scale = 2;
    sky.clouds.SetAmbientBrightness(0.7);
    sky.clouds.SetCloudDensity(1);
    sky.atmosphere.SetLightFilterIntensity(0.3);

    sky::PointLightSource* redStar = new sky::PointLightSource();
    redStar->colorSpectrum = db->GetSpectrum("RGB");
    redStar->intensity = 1.5;
    redStar->pos = std::pair<double, double>(5, 5);
    redStar->normalizedColor = std::vector<double>{ 1, .3, .1 };

    sky.lightSources.push_back(redStar);

    sky::PointLightSource* blueStar = new sky::PointLightSource();
    blueStar->colorSpectrum = db->GetSpectrum("RGB");
    blueStar->intensity = 1.0;
    blueStar->pos = std::pair<double, double>(-6, -5);

    blueStar->normalizedColor = std::vector<double>{ .3, .5, 1 };

    sky.lightSources.push_back(blueStar);

    composite_camera::CompositeCamera camera =
        composite_camera::CompositeCamera::CreateWithLimitedDynamicRange(
        db->GetSpectrum("RGB"), std::pair<unsigned int, unsigned int>(600, 600),
            std::pair<double, double>(0, 1));

    composite::RawCompositeImage* rawRgbImage =
        camera.Capture(&sky, std::pair<double, double>(-6, -6),
        std::pair<double, double>(12, 12));

    composite_processing::RGBACompositeImage* rgbaImage =
        composite_processing::Raw3c1aToRGBAConverter::ConvertRaw3c1aToRGBA(
            rawRgbImage);

    exporters::RGBAComposer::ExportImage(rgbaImage,
        "D:\\Education\\Bachelor Diploma\\Test Results\\0_rgb.png");

    camera.dynamicRange = std::pair<double, double>(0.5, 1.5);

    composite::RawCompositeImage* rawRgbHighShutterImage =
        camera.Capture(&sky, std::pair<double, double>(-6, -6),
            std::pair<double, double>(12, 12));

    composite_processing::RGBACompositeImage* highShutterImage =
        composite_processing::Raw3c1aToRGBAConverter::ConvertRaw3c1aToRGBA(
            rawRgbHighShutterImage);

    exporters::RGBAComposer::ExportImage(highShutterImage,
        "D:\\Education\\Bachelor Diploma\\Test Results\\0_rgb_high_shutter.png");
        
    camera.dynamicRange = std::pair<double, double>(0, 1);
    camera.targetSpectrum = db->GetSpectrum("Hubble");

    composite::RawCompositeImage* rawHubbleImage =
        camera.Capture(&sky, std::pair<double, double>(-6, -6),
            std::pair<double, double>(12, 12));

    composite_processing::RGBACompositeImage* hubbleImage =
        composite_processing::Raw3c1aToRGBAConverter::ConvertRaw3c1aToRGBA(
            rawHubbleImage);

    exporters::RGBAComposer::ExportImage(hubbleImage,
        "D:\\Education\\Bachelor Diploma\\Test Results\\0_hubble.png");

    return 0;
}


