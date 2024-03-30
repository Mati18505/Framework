#include "pch.h"
#include "Layer.h"
#include <cassert>
#include <ranges>

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
	void LayerStack::OnUpdate() const
	{
		for (const auto& layer : layers)
			layer->OnUpdate();
	}
	void LayerStack::OnEvent(Event& e) const
	{
		for (const auto& layer : layers | std::views::reverse)
			layer->OnEvent(e);
	}
	void LayerStack::OnRender() const
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