#include "eepch.h"
#include "LayerStack.h"


namespace Ellie{

	Ellie::LayerStack::LayerStack()
	{
	}

	Ellie::LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void Ellie::LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void Ellie::LayerStack::PushOverlay(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void Ellie::LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(begin(), end(), layer);
		if(it != end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void Ellie::LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != end())
		{
			m_Layers.erase(it);
		}
	}

}
