#include "pch.h"
#include "Window.h"
#include "Platform/Platform.h"
#include "Graphics.h"
#include "ioc/Container.h"

#define WND_EXCEPTION(message) Window::Exception(__LINE__, __FILE__, message)

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

			if (e.GetType() == Event::Type::FramebufferSizeChanged)
				gfx->ResizeFramebuffer(e.GetFramebufferWidth(), e.GetFramebufferHeight());

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
	std::shared_ptr<Window> Window::Create(const WindowDesc& desc)
	{
		return ioc::Container::Get().Resolve<Window>({ desc });
	}
	Framework::Graphics& Window::Gfx() const
	{
		if (!gfx)
			throw WND_EXCEPTION("Graphics is not initialized.");
		return *gfx;
	}
}
