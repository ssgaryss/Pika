#include "pkpch.h"
#include "LayerStack.h"

Pika::LayerStack::~LayerStack()
{
	for (auto it : m_pLayers) {
		delete it;
	}
}

void Pika::LayerStack::pushLayer(Layer* vLayer)
{
	m_pLayers.emplace(m_pLayers.begin() + m_LayerInsertIndex, vLayer);
	m_LayerInsertIndex++;
}

void Pika::LayerStack::popLayer(Layer* vLayer)
{
	auto it = std::find(m_pLayers.begin(), m_pLayers.begin() + m_LayerInsertIndex, vLayer);
	if (it != m_pLayers.begin() + m_LayerInsertIndex) {
		m_pLayers.erase(it);
		m_LayerInsertIndex--;
	}
}

void Pika::LayerStack::pushOverlay(Layer* vLayer)
{
	m_pLayers.emplace_back(vLayer);
}

void Pika::LayerStack::popOverlay(Layer* vLayer)
{
	auto it = std::find(m_pLayers.begin() + m_LayerInsertIndex, m_pLayers.end(), vLayer);
	if (it != m_pLayers.end()) {
		m_pLayers.erase(it);
	}
}
