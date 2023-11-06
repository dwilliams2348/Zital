#include "ZTpch.h"
#include "LayerStack.h"


namespace Zital
{

	LayerStack::LayerStack()
	{
		mLayerInsert = mLayers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* _layer)
	{
		mLayerInsert = mLayers.emplace(mLayerInsert, _layer);
	}

	void LayerStack::PushOverlay(Layer* _overlay)
	{
		mLayers.emplace_back(_overlay);
	}

	void LayerStack::PopLayer(Layer* _layer)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), _layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
			mLayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* _overlay)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), _overlay);
		if (it != mLayers.end())
			mLayers.erase(it);
	}

}