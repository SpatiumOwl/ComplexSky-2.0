#include "ColorSpectrumDB.h"

namespace cs
{
	namespace color_spectrum
	{
		ColorSpectrumDB::ColorSpectrumDB()
		{
			spectrums = map <string, ColorSpectrum*>();
			converters = map <pair<string, string>, ColorSpectrumConverter*>();
		}		

		void ColorSpectrumDB::AddSpectrum(ColorSpectrum* colorSpectrum)
		{
			if (SpectrumPresent(colorSpectrum->name))
				spectrums[colorSpectrum->name] = colorSpectrum;
		}
		ColorSpectrum* ColorSpectrumDB::GetSpectrum(string name)
		{
			if (SpectrumPresent(name))
				return spectrums[name];
			return nullptr;
		}
		bool ColorSpectrumDB::SpectrumPresent(string name)
		{
			return (spectrums.count(name) > 0);
		}

		ColorSpectrumConverter* ColorSpectrumDB::GetConverter(string source, string target)
		{
			if (ConverterPresent(source, target))
				return converters[pair<string, string>(source, target)];

			if (SpectrumPresent(source) && SpectrumPresent(target))
			{
				GenerateConverter(spectrums[source], spectrums[target]);
				return converters[pair<string, string>(source, target)];
			}

			return nullptr;
		}
		bool ColorSpectrumDB::ConverterPresent
		(string source, string target)
		{
			return (converters.count(pair<string, string>
				(source, target)) > 0);
		}		
		void ColorSpectrumDB::GenerateConverter
		(const ColorSpectrum* source, const ColorSpectrum* target)
		{
			converters[pair<string, string>(source->name, target->name)] =
				new ColorSpectrumConverter(source, target);
		}
		
	}
}