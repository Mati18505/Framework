#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Layer.h"
#include "KeyCodes.h"
#include "Logger.h"
#include "ImGuiLayer.h"
#include <imgui.h>

using namespace Framework;

class ImGuiTest : public Framework::Layer {
public:

	void OnAttach() override {
		Window::WindowDesc desc;
		window = Window::Create(desc);
		window->eventHandler = BIND_APPLICATION_EVENT_HANDLER();

		imGuiLayer = std::make_shared<ImGuiLayer>(window);
		Application::Get().GetLayerStack().PushLayer(imGuiLayer);
	}

	void OnUpdate() override {

		window->PollEvents();

		if (window->ShouldClose())
			Framework::Application::Get().Stop();
		else
			window->DispatchEvents();
	}
	void OnRender() override {
		window->Gfx().StartFrame();
		window->Gfx().ClearBuffer(1.0f, 1.0f, 0.5f);

		imGuiLayer->Begin();
		ImGui::ShowDemoWindow();
		imGuiLayer->End();

		window->Gfx().EndFrame();
	}
	void OnEvent(Framework::Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Keyboard::Event>(std::bind_front(&ImGuiTest::HandleKeyboardEvent, this));
	}

	void OnDetach() noexcept override {
	}

	bool HandleKeyboardEvent(Keyboard::Event& e) {
		if (e.GetCode() == FW_KEY_ENTER && e.IsPress())
		{
			CustomEvent e("NextLayerEvent");
			Application::Get().OnEvent(e);
		}
		return true;
	}

	std::shared_ptr<Window> window;
	std::shared_ptr<ImGuiLayer> imGuiLayer;
};