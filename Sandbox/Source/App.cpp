#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Platform/PlatformFactory.h"
#include "Layer.h"

using namespace Framework;

class SandboxLayer : public Framework::Layer {
public:
	~SandboxLayer() override = default;
	void OnAttach() override { 
		std::cout << "On Attach" << std::endl;

		Window::WindowDesc desc;
		desc.title = "Title";

		window = PlatformFactory::CreateWindow(desc);

		Window::WindowCallbacks windowCallbacks;

		windowCallbacks.windowShouldClose = []() {
			Application::Get().Stop();
		};
		window->SetWindowCallbacks(windowCallbacks);
	}
	void OnUpdate() override { 
		window->PollEvents();	
	}
	void OnRender() override { 
		/*
		for (auto window : windows)
			window->SwapBuffers();*/
	}
	void OnEvent() override { 
		std::cout << "On Event" << std::endl;
	}
	void OnDetach() noexcept override {
		std::cout << "On Detach" << std::endl;
	}
private:
	std::unique_ptr<Window> window;
};

std::unique_ptr<Framework::Application> Framework::CreateApplication()
{
	auto app = std::make_unique<Application>();
	
	std::shared_ptr<Layer> appLayer = std::make_shared<SandboxLayer>();
	app->GetLayerStack().PushLayer(appLayer);

	return app;
}