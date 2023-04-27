#pragma once
#include<map>
#include<utility>
#include<string>
#include "ColorSpectrumConverter.h"
#include "ColorSpectrum.h"

using namespace std;

namespace cs
{
	namespace color_spectrum
	{
		class ColorSpectrumDB
		{
		private:
			static ColorSpectrumDB* db;
			map <string, ColorSpectrum*> spectrums;
			map <pair<string, string>, 
				ColorSpectrumConverter*> converters;

			ColorSpectrumDB();
			
			bool ConverterPresent
			(string source, string target);
			
			void GenerateConverter
			(ColorSpectrum* source, ColorSpectrum* target);
			
			bool SpectrumPresent(string name);

		public:
			static inline ColorSpectrumDB* GetInstance()
			{
				if (ColorSpectrumDB::db == nullptr)
					ColorSpectrumDB::db = new ColorSpectrumDB();

				return db;
			}
			
			void AddSpectrum(ColorSpectrum* colorSpectrum);
			ColorSpectrumConverter* GetConverter(string source, string target);
			ColorSpectrum* GetSpectrum(string name);
		};
	}
}