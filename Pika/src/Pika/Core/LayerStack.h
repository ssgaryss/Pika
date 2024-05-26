#pragma once

#include "Pika/Core/Layer.h"

namespace Pika
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();
		void pushLayer(Layer* vLayer);
		void popLayer(Layer* vLayer);
		void pushOverlay(Layer* vLayer);
		void popOverlay(Layer* vLayer);

		inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		inline std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		inline std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
		inline std::vector<Layer*>::const_iterator cbegin() { return m_Layers.cbegin(); }
		inline std::vector<Layer*>::const_iterator cend() { return m_Layers.cend(); }
		inline std::vector<Layer*>::const_reverse_iterator crbegin() { return m_Layers.crbegin(); }
		inline std::vector<Layer*>::const_reverse_iterator crend() { return m_Layers.crend(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0; //push layer to the index, push overlay layer to the back 
	};
}

