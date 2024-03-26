#pragma once
#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Layer.h"
#include "Graphics.h"

class MultipleWindowsTest : public Framework::Layer {
public:

	void OnAttach() override {
		using namespace Framework;
		
		Window::WindowDesc desc;
		desc.mode = Window::Mode::Windowed;

		windows.reserve(windowsCount);
		for (int i = 0; i < windowsCount; i++)
		{
			windows.push_back(Window::Create(desc));
			windows[i]->eventHandler = [this, i](Event& e) { OnEventMultiWindow(i, e); };
		}
	}
	void OnUpdate() override {
		for (auto it = windows.begin(); it != windows.end();)
		{
			const auto& window = *it;

			window->PollEvents();

			if (window->ShouldClose())
			{
				openedWindowsCount--;
				if (openedWindowsCount == 0)
					Framework::Application::Get().Stop();
				it = windows.erase(it);
				continue;
			}
			else
				it++;

			window->DispatchEvents();
		}
	}
	void OnRender() override {
		for (int i = 0; i < windows.size(); i++)
		{
			const auto& window = windows.at(i);

			window->Gfx().StartFrame();
			window->Gfx().ClearBuffer(0.0f, 1.0f, 1.0f);

			window->Gfx().EndFrame();
		}
	}
	void OnEvent(Event& e) override {}

	void OnEventMultiWindow(int windowID, Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Keyboard::Event>([windowID](Keyboard::Event& e) {
			if (e.GetCode() == FW_KEY_ENTER && e.IsPress())
			{
				LOG_INFO("Enter pressed on window {}.", windowID);
				return true;
			}
			return false;
		});
	}
	void OnDetach() noexcept override {}

private:
	std::vector<std::shared_ptr<Framework::Window>> windows;
	int windowsCount = 2;
	int openedWindowsCount = windowsCount;
};

