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

		Window::KeyboardCallbacks kc;
		kc.keyPressed = [](int keyCode, bool repeated) {
			if (keyCode == 257)
				Application::Get().GetLayerStack().OnEvent();
			std::cout << "Keyboard Key Pressed: " << keyCode << ", repeated: " << repeated << std::endl;
		};
		kc.keyReleased = [](int keyCode) {
			std::cout << "Keyboard Key Released: " << keyCode << std::endl;
		};
		kc.characterWrite = [](char32_t character) {
			std::wcout << "Keyboard Character: " << (wchar_t)character << std::endl;
		};
		window->SetKeyboardCallbacks(kc);


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
	}
	void OnUpdate() override {
		window->PollEvents();
	}
	void OnRender() override {}
	void OnEvent() override {}
	void OnDetach() noexcept override {}

	std::shared_ptr<Framework::Window> window;
};