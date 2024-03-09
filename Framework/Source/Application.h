#pragma once
#include "Layer.h"

namespace Framework {
	class Application {
	public:
		Application();
		~Application();
		void Run();
		LayerStack& GetLayerStack() { return layerStack; }
		void Stop() { running = false; }
		static Application& Get();
	private:
		LayerStack layerStack;
		bool running = true;
	};
	std::unique_ptr<Application> CreateApplication();
}
