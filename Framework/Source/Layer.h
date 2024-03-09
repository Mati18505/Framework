#pragma once
#include <queue>

namespace Framework
{
	class Layer {
	public:
		virtual ~Layer() = default;
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent() = 0;
		virtual void OnDetach() noexcept = 0;
	};

	class LayerStack {
	public:
		void PushLayer(std::shared_ptr<Layer> layer);
		void PopLayer();
		void OnUpdate();
		void OnEvent();
		void OnRender();
		void Flush();
		void Update();
		~LayerStack();
	private:
		std::vector<std::shared_ptr<Layer>> layers;
		std::queue<std::function<void()>> delayed;
	};
}