#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Layer.h"
#include <Logger.h>
#include <Graphics.h>
#include <imgui.h>
#include <ImGuiLayer.h>

using namespace Framework;

class SandboxLayer : public Framework::Layer {
public:
	~SandboxLayer() override = default;
	void OnAttach() override { 
		LOG_TRACE("On Attach");

		Window::WindowDesc desc{
			.title = "Title"
		};

		window = Window::Create(desc);
		window->eventHandler = BIND_APPLICATION_EVENT_HANDLER();

		imguiLayer = std::make_shared<ImGuiLayer>(window);
		Application::Get().GetLayerStack().PushLayer(imguiLayer);

		LOG_CORE_TRACE("Test core trace.");
		LOG_CORE_INFO("Test core info.");
		LOG_CORE_WARN("Test core warn.");
		LOG_CORE_ERROR("Test core error.");
		LOG_CORE_CRITICAL("Test core critical.");
		
		LOG_TRACE("Test app trace.");
		LOG_INFO("Test app info.");
		LOG_WARN("Test app warn.");
		LOG_ERROR("Test app error.");
		LOG_CRITICAL("Test app critical.");
	}
	void OnUpdate() override { 
		window->PollEvents();	
		window->DispatchEvents();
		LOG_ONCE("Once");
	}
	void OnRender() override { 
		window->Gfx().StartFrame();
		window->Gfx().ClearBuffer(0.5f, 0.7f, 0.6f);
		imguiLayer->Begin();
		ImGui::ShowDemoWindow();
		ImGui::Begin("test");
		ImGui::Button("przycisk");
		ImGui::End();
		imguiLayer->End();
		window->Gfx().EndFrame();
	}
	void OnEvent(Event& e) override { 
		LOG_ONCE("On Event");
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
		LOG_TRACE("On Detach");
	}
private:
	std::shared_ptr<ImGuiLayer> imguiLayer;
	std::shared_ptr<Window> window;
};

std::unique_ptr<Framework::Application> Framework::CreateApplication()
{
	auto app = std::make_unique<Application>();
	
	std::shared_ptr<Layer> appLayer = std::make_shared<SandboxLayer>();
	app->GetLayerStack().PushLayer(appLayer);

	return app;
}