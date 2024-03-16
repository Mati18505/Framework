#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Layer.h"

using namespace Framework;

class SandboxLayer : public Framework::Layer {
public:
	~SandboxLayer() override = default;
	void OnAttach() override { 
		std::cout << "On Attach" << std::endl;

		Window::WindowDesc desc;
		desc.title = "Title";

		window = Window::Create(desc);
		window->eventHandler = BIND_APPLICATION_EVENT_HANDLER();
	}
	void OnUpdate() override { 
		window->PollEvents();	
		window->DispatchEvents();
	}
	void OnRender() override { 
		/*
		for (auto window : windows)
			window->SwapBuffers();*/
	}
	void OnEvent(Event& e) override { 
		std::cout << "On Event" << std::endl;
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Window::Event>([](Window::Event& e) {
			if (e.GetType() == Window::Event::Type::ShouldClose)
			{
				Application::Get().Stop();
				return true;
			}
			return false;
		});
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