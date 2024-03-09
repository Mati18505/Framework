#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Platform/PlatformFactory.h"
#include "Layer.h"
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class MultipleWindowsTest : public Framework::Layer {
public:

	void OnAttach() override {
		using namespace Framework;
		
		Window::WindowDesc desc;
		desc.mode = Window::Mode::Windowed;

		windows.reserve(windowsCount);
		for (int i = 0; i < windowsCount; i++)
		{
			windows.push_back(PlatformFactory::CreateWindow(desc));
			Window::MouseCallbacks mc;
		}
	}
	void OnUpdate() override {
		windows.at(0)->PollEvents();

		for (auto it = windows.begin(); it != windows.end();)
		{
			const auto& window = *it;
			if (window->ShouldClose())
			{
				openedWindowsCount--;
				if (openedWindowsCount == 0)
					Framework::Application::Get().Stop();
				it = windows.erase(it);
			}
			else
				it++;
		}
	}
	void OnRender() override {}
	void OnEvent() override {}
	void OnDetach() noexcept override {}

private:
	std::vector<std::shared_ptr<Framework::Window>> windows;
	int windowsCount = 2;
	int openedWindowsCount = windowsCount;
};

