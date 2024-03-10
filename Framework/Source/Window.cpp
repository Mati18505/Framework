#include "pch.h"
#include "Window.h"

namespace Framework {
	void Window::DispatchEvents()
	{
		assert(eventHandler != nullptr);
		while (!kbd.KeyIsEmpty())
		{
			if (auto opt = kbd.ReadKey())
				eventHandler(opt.value());
		}
		while (!mouse.IsEmpty())
		{
			if (auto opt = mouse.Read())
				eventHandler(opt.value());
		}
		while (!windowEvents.empty())
		{
			Window::Event e = windowEvents.front();
			windowEvents.pop();
			eventHandler(e);
		}
	}
	void Window::TrimEventBuffer()
	{
		while (windowEvents.size() > 16)
		{
			windowEvents.pop();
		}
	}
}
