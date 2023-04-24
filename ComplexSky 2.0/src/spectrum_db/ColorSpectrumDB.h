#pragma once
#include<map>
#include<utility>
#include<string>
#include "ColorSpectrumConverter.h"
#include "ColorSpectrum.h"

using namespace std;

namespace cs
{
	namespace spectrum_db
	{
		class ColorSpectrumDB
		{
		private:
			static ColorSpectrumDB* db;
			map <string, ColorSpectrum> spectrums;
			map <pair<string, string>, 
				ColorSpectrumConverter> converters;

			//Uninline
			inline ColorSpectrumDB()
			{
				spectrums = map <string, ColorSpectrum>();
				converters = map <pair<string, string>,	ColorSpectrumConverter>();
			}
			//Unlinline
			inline bool ConverterGenerated
			(ColorSpectrum* source, ColorSpectrum* target)
			{
				return (converters.count(pair<string, string>
					(source->name, target->name)) > 0);
			}
			//Uninline
			inline void GenerateConverter
			(ColorSpectrum* source, ColorSpectrum* target)
			{
				converters[pair<string, string> (source->name, target->name)] = 
					ColorSpectrumConverter(source, target);
			}
			//Uninline
			inline bool SpectrumPresent(string name)
			{
				return (spectrums.count(name) > 0);
			}

		public:
			inline ColorSpectrumDB* GetInstance()
			{
				if (ColorSpectrumDB::db == nullptr)
					ColorSpectrumDB::db = new ColorSpectrumDB();

				return db;
			}
			//Uninline
			inline void AddSpectrum(ColorSpectrum colorSpectrum)
			{
				if (SpectrumPresent(colorSpectrum.name))
					spectrums[colorSpectrum.name] = colorSpectrum;
			}
			ColorSpectrumConverter* GetConverter(string source, string target);
			ColorSpectrum* GetSpectrum(string name);
		};
	}
}