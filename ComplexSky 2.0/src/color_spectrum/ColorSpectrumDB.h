#pragma once
#include <map>
#include <utility>
#include <string>
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
			static ColorSpectrumDB* instance;

			map <string, ColorSpectrum*> spectrums;
			map <pair<string, string>, 
				ColorSpectrumConverter*> converters;

			ColorSpectrumDB();
			
			bool ConverterPresent
			(string source, string target);
			
			void GenerateConverter
			(const ColorSpectrum* source, const ColorSpectrum* target);
			
			bool SpectrumPresent(string name);

		public:
			ColorSpectrumDB(const ColorSpectrumDB& obj)
				= delete;

			void operator=(const ColorSpectrumDB&) = delete;

			static inline ColorSpectrumDB* GetInstance()
			{
				if (instance == nullptr)
					instance = new ColorSpectrumDB();

				return instance;
			}
			
			void AddSpectrum(ColorSpectrum* colorSpectrum);
			ColorSpectrumConverter* GetConverter(string source, string target);
			ColorSpectrum* GetSpectrum(string name);
		};
	}
}