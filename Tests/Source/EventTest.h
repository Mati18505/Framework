#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Platform/PlatformFactory.h"
#include "Layer.h"
#include "KeyCodes.h"

using namespace Framework;

class EventTest : public Framework::Layer {
public:

	void OnAttach() override {

		Window::WindowDesc desc;
		window = PlatformFactory::CreateWindow(desc);
		window->eventHandler = BIND_APPLICATION_EVENT_HANDLER();
	}

	int wheel = 0;

	void OnUpdate() override {

		window->PollEvents();

		if(auto opt = window->kbd.ReadChar())
			std::cout<<opt.value()<<std::endl;

		if (window->kbd.IsKeyPressed(87))
			std::cout << "Forward" << std::endl;

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
			std::cout << "Right mouse button pressed" << std::endl;
			break;
		case Mouse::Event::Type::LPress:
			std::cout << "Left mouse button pressed" << std::endl;
			break;
		case Mouse::Event::Type::RRelease:
			std::cout << "Right mouse button released" << std::endl;
			break;
		case Mouse::Event::Type::LRelease:
			std::cout << "Left mouse button released" << std::endl;
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
			std::cout << "Window should close!" << std::endl;
			Application::Get().Stop();
			break;
		case Window::Event::Type::FocusEnter:
			std::cout << "Window focus enter" << std::endl;
			break;
		case Window::Event::Type::FocusLeave:
			std::cout << "Window focus leave" << std::endl;
			break;
		case Window::Event::Type::FramebufferSizeChanged:
			std::cout << "Framebuffer Size Changed: " << e.GetFramebufferWidth() << "_" << e.GetFramebufferHeight() << std::endl;
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