#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Layer.h"
#include "KeyCodes.h"
#include <Logger.h>

using namespace Framework;

class EventTest : public Framework::Layer {
public:

	void OnAttach() override {
		Window::WindowDesc desc;
		window = Window::Create(desc);
		window->eventHandler = BIND_APPLICATION_EVENT_HANDLER();
	}

	int wheel = 0;

	void OnUpdate() override {

		window->PollEvents();

		if(auto opt = window->kbd.ReadChar())
			LOG_TRACE("{}", opt.value());

		if (window->kbd.IsKeyPressed(87))
			LOG_TRACE("Forward");

		window->DispatchEvents();
	}
	void OnRender() override {}
	void OnEvent(Framework::Event& e) override 
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Mouse::Event>(std::bind(&EventTest::MouseEventTest, this, std::placeholders::_1));
		dispatcher.Dispatch<Window::Event>(std::bind(&EventTest::WindowEventTest, this, std::placeholders::_1));
		dispatcher.Dispatch<Keyboard::Event>(std::bind(&EventTest::KeyboardEventTest, this, std::placeholders::_1));
	}
	void OnDetach() noexcept override {}

	std::shared_ptr<Window> window;

	bool MouseEventTest(Mouse::Event& e)
	{
		switch (e.GetType())
		{
		case Mouse::Event::Type::Leave:
			window->SetTitle("Gone!");
			break;
		case Mouse::Event::Type::Move:
		{
			std::ostringstream ss;
			ss << e.GetPosX() << "_" << e.GetPosY();
			window->SetTitle(ss.str());
			break;
		}
		case Mouse::Event::Type::WheelUp:
			wheel++;
			window->SetTitle(std::to_string(wheel));
			break;
		case Mouse::Event::Type::WheelDown:
			wheel--;
			window->SetTitle(std::to_string(wheel));
			break;
		case Mouse::Event::Type::RPress:
			LOG_TRACE("Right mouse button pressed");
			break;
		case Mouse::Event::Type::LPress:
			LOG_TRACE("Left mouse button pressed");
			break;
		case Mouse::Event::Type::RRelease:
			LOG_TRACE("Right mouse button released");
			break;
		case Mouse::Event::Type::LRelease:
			LOG_TRACE("Left mouse button released");
			break;
		default:
			break;
		}
		return true;
	}

	bool WindowEventTest(Window::Event& e)
	{
		switch (e.GetType())
		{
		case Window::Event::Type::ShouldClose:
			LOG_TRACE("Window should close!");
			Application::Get().Stop();
			break;
		case Window::Event::Type::FocusEnter:
			LOG_TRACE("Window focus enter");
			break;
		case Window::Event::Type::FocusLeave:
			LOG_TRACE("Window focus leave");
			break;
		case Window::Event::Type::FramebufferSizeChanged:
			LOG_TRACE("Framebuffer Size Changed: {0}_{1}", e.GetFramebufferWidth(), e.GetFramebufferHeight());
			break;
		default:
			break;
		}
		return true;
	}

	bool KeyboardEventTest(Keyboard::Event& e)
	{
		if (e.GetCode() == FW_KEY_ENTER && e.IsPress())
		{
			CustomEvent e("NextLayerEvent");
			Application::Get().OnEvent(e);
		}
		return true;
	}
};