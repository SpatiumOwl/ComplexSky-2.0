#include "ColorSpectrumDB.h"

namespace cs
{
	namespace color_spectrum
	{
		ColorSpectrumDB::ColorSpectrumDB()
		{
			spectrums = map <string, ColorSpectrum>();
			converters = map <pair<string, string>, ColorSpectrumConverter>();
		}
		bool ColorSpectrumDB::ConverterGenerated
		(ColorSpectrum* source, ColorSpectrum* target)
		{
			return (converters.count(pair<string, string>
				(source->name, target->name)) > 0);
		}
		void ColorSpectrumDB::GenerateConverter
		(ColorSpectrum* source, ColorSpectrum* target)
		{
			converters[pair<string, string>(source->name, target->name)] =
				ColorSpectrumConverter(source, target);
		}
		bool ColorSpectrumDB::SpectrumPresent(string name)
		{
			return (spectrums.count(name) > 0);
		}
		void ColorSpectrumDB::AddSpectrum(ColorSpectrum colorSpectrum)
		{
			if (SpectrumPresent(colorSpectrum.name))
				spectrums[colorSpectrum.name] = colorSpectrum;
		}
		ColorSpectrumConverter* ColorSpectrumDB::GetConverter(string source, string target)
		{

		}
	}
}