#include "eepch.h"
#include "LayerStack.h"


namespace Ellie{

	Ellie::LayerStack::LayerStack()
	{
		m_LayerInsert = begin();
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
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
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
			m_Layers.erase(m_LayerInsert);
			m_LayerInsert--;
		}
	}

	void Ellie::LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != end())
		{
			m_Layers.erase(end());
		}
	}

}
