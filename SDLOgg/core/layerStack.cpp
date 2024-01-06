#include "core/layerStack.hpp"

namespace fs
{
    
	LayerStack::~LayerStack()
	{
		for(Layer* layer : m_layerstack  )
			delete layer ; //删除所有层 -> layerStack拥有layer所有权
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layerstack.emplace(m_layerstack.begin()+m_LayerInsertIndex,layer); 
		m_LayerInsertIndex++;	
	}

	void LayerStack::PushOverLayer(Layer* layer)
	{

		m_layerstack.emplace_back(layer); //覆盖层
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layerstack.begin(), m_layerstack.end(), layer);//弹出,layerstack失去其所有权,layer不会随之删除	
		if (it != m_layerstack.end()) 
		{
			m_layerstack.erase(it);
			m_LayerInsertIndex--;
		}

	}

	void LayerStack::PopOverLayer(Layer* layer)
	{
		auto it = std::find(m_layerstack.begin(), m_layerstack.end(), layer);
		if (it != m_layerstack.end())
		{
			m_layerstack.erase(it);
		}
	}

} // namespace fs
