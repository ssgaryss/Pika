#include "pkpch.h"
#include "LayerStack.h"

Pika::LayerStack::~LayerStack()
{
	for (auto it : m_Layers) {
		it->onDetach();
		delete it;
	}
}

void Pika::LayerStack::pushLayer(Layer* vLayer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, vLayer);
	vLayer->onAttach();
	m_LayerInsertIndex++;
}

void Pika::LayerStack::popLayer(Layer* vLayer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, vLayer);
	if (it != m_Layers.begin() + m_LayerInsertIndex) {
		vLayer->onDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void Pika::LayerStack::pushOverlay(Layer* vLayer)
{
	m_Layers.emplace_back(vLayer);
	vLayer->onAttach();
}

void Pika::LayerStack::popOverlay(Layer* vLayer)
{
	auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), vLayer);
	if (it != m_Layers.end()) {
		vLayer->onDetach();
		m_Layers.erase(it);
	}
}
