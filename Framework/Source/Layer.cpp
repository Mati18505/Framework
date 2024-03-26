#include "pch.h"
#include "Layer.h"
#include <cassert>

namespace Framework
{
	void LayerStack::PushLayer(std::shared_ptr<Layer> layer)
	{
		delayed.push([&, layer]() {
			layers.push_back(layer);
			layer->OnAttach();
		});
	}
	void LayerStack::PopLayer()
	{
		delayed.push([&]() {
			assert(!layers.empty());

			layers.back()->OnDetach();
			layers.pop_back();
		});
	}
	void LayerStack::OnUpdate()
	{
		for (const auto& layer : layers)
			layer->OnUpdate();
	}
	void LayerStack::OnEvent(Event& e)
	{
		for (auto it = layers.rbegin(); it != layers.rend(); it++)
			it->get()->OnEvent(e);
	}
	void LayerStack::OnRender()
	{
		for (const auto& layer : layers)
			layer->OnRender();
	}
	void LayerStack::Flush()
	{
		Update();

		for (const auto& layer : layers)
			layer->OnDetach();
		
		layers.clear();
	}
	void LayerStack::Update()
	{
		while (!delayed.empty())
		{
			std::function<void()> func = delayed.front();
			delayed.pop();
			func();
		}
	}
	LayerStack::~LayerStack()
	{
		Flush();
	}
}