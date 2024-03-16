#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Layer.h"
#include "KeyCodes.h"
#include "ConsoleTerminal.h"

using namespace Framework;

class ConsoleTest : public Framework::Layer {
public:

	ConsoleTerminal console;

	void OnAttach() override {

		Window::WindowDesc desc;
		window = Window::Create(desc);
		window->eventHandler = BIND_APPLICATION_EVENT_HANDLER();

		console.SetMessageSendCallback([this](const std::string& message) {
				console.AddMessage(message);
		});
	}

	int wheel = 0;

	void OnUpdate() override {

		window->PollEvents();

		window->DispatchEvents();
	}
	void OnRender() override {}
	void OnEvent(Framework::Event& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Window::Event>(std::bind(&ConsoleTest::HandleWindowEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<Keyboard::Event>(std::bind(&ConsoleTest::HandleKeyboardEvent, this, std::placeholders::_1));
	}
	void OnDetach() noexcept override {}

	bool HandleWindowEvent(Window::Event& e) {
		if (e.GetType() == Window::Event::Type::ShouldClose)
			Application::Get().Stop();
		return true;
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
};