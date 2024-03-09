#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Platform/PlatformFactory.h"
#include "Layer.h"

class EventTest : public Framework::Layer {
public:

	void OnAttach() override {
		using namespace Framework;

		Window::WindowDesc desc;
		window = PlatformFactory::CreateWindow(desc);
		window->SetDefaultCallbacks();

		Window::WindowCallbacks wc = window->GetWindowCallbacks();
		wc.windowSizeChanged = [](int width, int height) {
			std::cout << "Window Size Changed: " << width << "_" << height << std::endl;
		};
		wc.framebufferSizeChanged = [](int width, int height) {
			std::cout << "Framebuffer Size Changed: " << width << "_" << height << std::endl;
		};
		wc.windowShouldClose = []() {
			std::cout << "Window should close!" << std::endl;
			Application::Get().Stop();
		};
		window->SetWindowCallbacks(wc);
	}

	int wheel = 0;

	void OnUpdate() override {
		using namespace Framework;

		window->PollEvents();
		if(auto opt = window->kbd.ReadChar())
			std::cout<<opt.value()<<std::endl;

		if (window->kbd.IsKeyPressed(87))
			std::cout << "Forward" << std::endl;

		if (auto opt = window->kbd.ReadKey())
		{
			if (opt.value().GetCode() == 257 && opt.value().IsPress())
				Application::Get().GetLayerStack().OnEvent();
		}

		while (!window->mouse.IsEmpty())
		{
			if (auto opt = window->mouse.Read())
			{
				Mouse::Event e = opt.value();

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
			}
		}
		
	}
	void OnRender() override {}
	void OnEvent() override {}
	void OnDetach() noexcept override {}

	std::shared_ptr<Framework::Window> window;
};