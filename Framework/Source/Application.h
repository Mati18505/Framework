#pragma once
#include "Layer.h"

#define BIND_APPLICATION_EVENT_HANDLER() std::bind(&Application::OnEvent, &Application::Get(), std::placeholders::_1)

namespace Framework {
	class Application {
	public:
		Application();
		virtual ~Application();
		virtual void Run();
		LayerStack& GetLayerStack() { return layerStack; }
		void Stop() { running = false; }
		static Application& Get();
		virtual void OnEvent(Event& e);
	private:
		LayerStack layerStack;
		bool running = true;
	};
	std::unique_ptr<Application> CreateApplication();
}
