#pragma once
#include <list>
#include <string>
#include <utility>

namespace cs
{
	namespace composite {
		enum class BlendingMode
		{
			CS_NORMAL,
			CS_SCREEN,
			CS_MULTIPLY
		};

		template<typename T>
		class Layer
		{
		protected:
			bool isFolder = 0;

			inline Layer(std::string name, BlendingMode blendingMode, bool isFolder) : 
				name(name), blendingMode(blendingMode), isFolder(isFolder) {}
			inline Layer() {}
		public:
			std::string name;
			BlendingMode blendingMode;

			inline bool IsFolder() { return isFolder; }

			inline ~Layer() {}
		};
		template<typename T>
		class ImageLayer : public Layer<T>
		{
		private:
			T image;
			bool isEmpty = 1;
		public:
			std::pair<int, int> offset;

			inline ImageLayer() : Layer<T>("", BlendingMode::CS_NORMAL, false) {}
			inline ImageLayer(std::string name, BlendingMode blendingMode) : Layer<T>(name, blendingMode, false) {}
			inline ~ImageLayer() {}

			inline T* GetImage() { return &image; }
			inline void SetImage(T* _image) { image = *_image; isEmpty = 0; }
			inline bool IsEmpty() { return isEmpty; }
		};
		template<typename T>
		class FolderLayer : public Layer<T>
		{
		public:
			std::list<Layer<T>*>* layers;

			inline FolderLayer() : Layer<T>("", BlendingMode::CS_NORMAL, true), 
				layers(new std::list<Layer<T>*>()) {}
			inline FolderLayer(std::string name, BlendingMode blendingMode) : Layer<T>(name, blendingMode, true), 
				layers(new std::list<Layer<T>*>()) {}
			inline ~FolderLayer()
			{
				for (auto layer = layers->begin(); layer != layers->end(); layer++)
					delete(*layer);
				delete(layers);
			}

			inline bool IsEmpty() { return layers->size(); }
		};
	}
}
