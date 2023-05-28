// ComplexSky 2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "math_tools/MatrixOperations.h"
#include "sky/Sky.h"
#include "color_spectrum/ColorSpectrumDB.h"
#include "composite_camera/CompositeCamera.h"
#include "composite_processing/Raw3c1aToRGBAConverter.h"
#include "exporters/RGBAComposer.h"
#include "exporters/PSDExporter.h"
#include "raw_texture_converters/HubbleToRawConverter.h"
#include "raw_texture_converters/RGBAToRawConverter.h"
#include "TestSamples.h"

using namespace cs;

void AddSpectrums(cs::color_spectrum::ColorSpectrumDB* db)
{
    color_spectrum::ColorSpectrum* rgb = new color_spectrum::ColorSpectrum();
    rgb->name = "RGB";

    color_spectrum::ColorSpectrumChannel* red = new color_spectrum::ColorSpectrumChannel();
    red->distanceBetweenValues = 25;
    red->func = std::vector<double>{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 8, 6, 6,
        6, 10, 1, 30,
        75, 50, 30, 3,
        0 };
    rgb->colorChannels.push_back(*red);

    color_spectrum::ColorSpectrumChannel* green = new color_spectrum::ColorSpectrumChannel();
    green->distanceBetweenValues = 25;
    green->func = std::vector<double>{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 9, 15, 38,
        50, 90, 100, 50,
        10, 3, 1, 0 };
    rgb->colorChannels.push_back(*green);

    color_spectrum::ColorSpectrumChannel* blue = new color_spectrum::ColorSpectrumChannel();
    blue->distanceBetweenValues = 25;
    blue->func = std::vector<double>{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 30, 85, 95,
        50, 9, 1, 0 };
    rgb->colorChannels.push_back(*blue);

    db->AddSpectrum(rgb);

    color_spectrum::ColorSpectrum* hubble = new color_spectrum::ColorSpectrum();
    hubble->name = "Hubble";

    color_spectrum::ColorSpectrumChannel* s2 = new color_spectrum::ColorSpectrumChannel();
    s2->distanceBetweenValues = 12.5;
    s2->func = std::vector<double>{
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2, 90, 2,
        0
    };
    hubble->colorChannels.push_back(*s2);

    color_spectrum::ColorSpectrumChannel* ha = new color_spectrum::ColorSpectrumChannel();
    ha->distanceBetweenValues = 12.5;
    ha->func = std::vector<double>{
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 90, 2, 0,
    };
    hubble->colorChannels.push_back(*ha);

    color_spectrum::ColorSpectrumChannel* o3 = new color_spectrum::ColorSpectrumChannel();
    o3->distanceBetweenValues = 12.5;
    o3->func = std::vector<double>{
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2,
        90, 2, 0
    };
    hubble->colorChannels.push_back(*o3);

    db->AddSpectrum(hubble);
}

void InitializeSky0(cs::sky::Sky& sky)
{
    sky.clouds.scale = 2;
    sky.clouds.SetAmbientBrightness(0.7);
    sky.clouds.SetCloudDensity(1);
    sky.atmosphere.SetLightFilterIntensity(0.3);
}

void InitializeSky1(cs::sky::Sky& sky)
{
    sky.clouds.scale = 2;
    sky.clouds.SetAmbientBrightness(0.05);
    sky.clouds.SetHighlightBrightness(3);
    sky.clouds.SetCloudDensity(1);
    sky.atmosphere.SetAmbientLightIntensity(0.05);
    sky.atmosphere.SetLightFilterIntensity(0.3);
}

void AddPointLights0(cs::color_spectrum::ColorSpectrumDB* db, cs::sky::Sky& sky)
{
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
}

void AddPointLights1(cs::color_spectrum::ColorSpectrumDB* db, cs::sky::Sky& sky)
{
    sky::PointLightSource* redStar = new sky::PointLightSource();
    redStar->colorSpectrum = db->GetSpectrum("RGB");
    redStar->intensity = 0.75;
    redStar->pos = std::pair<double, double>(7, 7);
    redStar->normalizedColor = std::vector<double>{ 1, .3, .1 };

    sky.lightSources.push_back(redStar);

    sky::PointLightSource* blueStar = new sky::PointLightSource();
    blueStar->colorSpectrum = db->GetSpectrum("RGB");
    blueStar->intensity = 0.5;
    blueStar->pos = std::pair<double, double>(-6, -7);

    blueStar->normalizedColor = std::vector<double>{ .3, .5, 1 };

    sky.lightSources.push_back(blueStar);
}

void AddTextureLights0(cs::color_spectrum::ColorSpectrumDB* db, cs::sky::Sky& sky)
{
    sky::TextureLightSource* galaxy = new sky::TextureLightSource();
    galaxy->texture = rtc::HubbleToRawConverter::ImportTexture(
        "D:\\Education\\Bachelor Diploma\\ComplexSky 2.0\\ComplexSky 2.0\\res\\Galaxy_0.png",
        std::pair<double, double>(0, 2));
    galaxy->pos = std::pair<double, double>(-1, -0.5);
    galaxy->size = std::pair<double, double>(4, 2.6);

    sky.lightSources.push_back(galaxy);
}

void AddTextureLights1(cs::color_spectrum::ColorSpectrumDB* db, cs::sky::Sky& sky)
{
    AddTextureLights0(db, sky);

    sky::TextureLightSource* nebula = new sky::TextureLightSource();
    nebula->texture = rtc::HubbleToRawConverter::ImportTexture(
        "D:\\Education\\Bachelor Diploma\\ComplexSky 2.0\\ComplexSky 2.0\\res\\Nebula.png",
        std::pair<double, double>(0, 2));
    nebula->pos = std::pair<double, double>(-6, -6);
    nebula->size = std::pair<double, double>(12, 8);

    sky.lightSources.push_back(nebula);
}

int main()
{
    color_spectrum::ColorSpectrumDB* db =
        color_spectrum::ColorSpectrumDB::GetInstance();

    AddSpectrums(db);
        
    sky::Sky sky;

    InitializeSky1(sky);

    AddPointLights1(db, sky);

    AddTextureLights1(db, sky);

    composite_camera::CompositeCamera camera =
        composite_camera::CompositeCamera::CreateWithLimitedDynamicRange(
        db->GetSpectrum("RGB"), std::pair<unsigned int, unsigned int>(600, 600),
            std::pair<double, double>(0, 0.3));

    composite::RawCompositeImage* rawRgbImage =
        camera.Capture(&sky, std::pair<double, double>(-6, -6),
        std::pair<double, double>(12, 12));

    composite_processing::RGBACompositeImage* rgbaImage =
        composite_processing::Raw3c1aToRGBAConverter::ConvertRaw3c1aToRGBA(
            rawRgbImage);

    exporters::RGBAComposer::ExportImage(rgbaImage,
        "D:\\Education\\Bachelor Diploma\\Test Results\\0_rgb.png");

    /*exporters::PSDExporter::ExportPSD(rgbaImage,
        "D:\\Education\\Bachelor Diploma\\Test Results\\0_rgb.psd");

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
        "D:\\Education\\Bachelor Diploma\\Test Results\\0_hubble.png");*/

    return 0;
}



