#pragma once
#include <list>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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

			inline Layer(std::string name, BlendingMode blendingMode) : name(name), blendingMode(blendingMode) {}
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
			cv::Vec2i offset;

			inline ImageLayer() { isFolder = 0; }
			inline ImageLayer(std::string name, BlendingMode blendingMode) : Layer(name, blendingMode) { isFolder = 0; }
			inline ~ImageLayer() {}

			inline T* GetImage() { return &image; }
			inline void SetImage(cv::Mat4d* image) { image = *image; isEmpty = 0; }
			inline bool IsEmpty() { return isEmpty; }
		};
		template<typename T>
		class FolderLayer : public Layer<T>
		{
		public:
			std::list<Layer<T>*>* layers;

			inline FolderLayer() : layers(new std::list<Layer<T>*>()) { isFolder = 1; }
			inline FolderLayer(std::string name, BlendingMode blendingMode) : Layer(name, blendingMode), layers(new std::list<Layer<T>*>()) { isFolder = 1; }
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
