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

		inline std::vector<Layer*>::iterator begin() { return m_pLayers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_pLayers.end(); }
		inline std::vector<Layer*>::reverse_iterator rbegin() { return m_pLayers.rbegin(); }
		inline std::vector<Layer*>::reverse_iterator rend() { return m_pLayers.rend(); }
		inline std::vector<Layer*>::const_iterator begin() const { return m_pLayers.cbegin(); }
		inline std::vector<Layer*>::const_iterator end() const { return m_pLayers.cend(); }
		inline std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_pLayers.crbegin(); }
		inline std::vector<Layer*>::const_reverse_iterator rend() const { return m_pLayers.crend(); }

	private:
		std::vector<Layer*> m_pLayers;
		unsigned int m_LayerInsertIndex = 0; //push layer to the index, push overlay layer to the back 
	};
}

