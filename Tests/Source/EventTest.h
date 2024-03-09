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

		Window::MouseCallbacks mc;
		mc.mousePosChange = [](int x, int y) {
			std::cout << "Mouse Pos Change: " << x << "_" << y << std::endl;
		};
		mc.mouseWheelDelta = [](int delta, int x, int y) {
			std::cout << "Mouse Wheel Delta: " << delta << ", at: " << x << "_" << y << std::endl;
		};
		mc.mouseEnterWindow = []() {
			std::cout << "Mouse Entered The Window!" << std::endl;
		};
		mc.mouseLeaveWindow = []() {
			std::cout << "Mouse Left The Window!" << std::endl;
		};
		mc.mouseButtonPressed = [](int buttonCode, int x, int y) {
			std::cout << "Mouse Button Pressed: " << buttonCode << ", at: " << x << "_" << y << std::endl;
		};
		mc.mouseButtonReleased = [](int buttonCode, int x, int y) {
			std::cout << "Mouse Button Released: " << buttonCode << ", at: " << x << "_" << y << std::endl;
		};
		window->SetMouseCallbacks(mc);

		Window::WindowCallbacks wc;
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

		window->SetDefaultCallbacks();
	}
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
	}
	void OnRender() override {}
	void OnEvent() override {}
	void OnDetach() noexcept override {}

	std::shared_ptr<Framework::Window> window;
};