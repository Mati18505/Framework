#pragma once
#include "Layer.h"
#include "Graphics.h"

struct ImGuiContext;

namespace Framework {
	class Window;
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer(std::weak_ptr<Window> window)
			:window(window)
		{}

		void OnAttach() override;
		void OnDetach() noexcept override;
		void OnEvent(Event& e) override;
		void OnUpdate() override {}
		void OnRender() override {}

		void Begin();
		void End();
	private:
		ImGuiContext* imGuiContext;
		std::weak_ptr<Window> window;

		Graphics& GetGfx() const;
	};
}