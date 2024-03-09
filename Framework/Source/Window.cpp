#include "pch.h"
#include "Window.h"

namespace Framework {
	void Window::SetDefaultCallbacks()
	{
		KeyboardCallbacks kc;
		kc.keyPressed = [&](int keyCode, bool repeated) {
			if (!kbd.AutorepeatIsEnabled() && repeated)
				return;
			kbd.OnKeyPressed(keyCode, repeated);
		};
		kc.keyReleased = [&](int keyCode) {
			kbd.OnKeyReleased(keyCode);
		};
		kc.characterWrite = [&](char32_t character) {
			kbd.OnCharacterWrite(character);
		};
		SetKeyboardCallbacks(kc);

		Window::MouseCallbacks mc;
		mc.mousePosChange = [&](int x, int y) {
			mouse.OnMouseMove(x, y);
		};
		mc.mouseWheelDelta = [&](int delta, int x, int y) {
			if(delta < 0)
				mouse.OnWheelDown();
			else if (delta > 0)
				mouse.OnWheelUp();
		};
		mc.mouseEnterWindow = [&]() {
			mouse.OnMouseEnter();
		};
		mc.mouseLeaveWindow = [&]() {
			mouse.OnMouseLeave();
		};
		mc.mouseButtonPressed = [&](int buttonCode, int x, int y) {
			switch (buttonCode)
			{
			case 0:
				mouse.OnLeftPressed();
				break;
			case 1:
				mouse.OnRightPressed();
				break;
			default:
				break;
			}
		};
		mc.mouseButtonReleased = [&](int buttonCode, int x, int y) {
			switch (buttonCode)
			{
			case 0:
				mouse.OnLeftReleased();
				break;
			case 1:
				mouse.OnRightReleased();
				break;
			default:
				break;
			}
		};
		SetMouseCallbacks(mc);

		WindowCallbacks wc;
		wc.focusChanged = [&](bool focused) {
			kbd.ClearState();
			if (focused)
			{
				Window::Event e(Window::Event::Type::FocusEnter);
				windowEvents.push(e);
				TrimEventBuffer();
			}
			else
			{
				Window::Event e(Window::Event::Type::FocusLeave);
				windowEvents.push(e);
				TrimEventBuffer();
			}
		};
		wc.framebufferSizeChanged = [&](int x, int y) {
			Window::Event e(Window::Event::Type::FramebufferSizeChanged, x, y);
			windowEvents.push(e);
			TrimEventBuffer();
		};
		wc.windowShouldClose = [&]() {
			Window::Event e(Window::Event::Type::ShouldClose);
			windowEvents.push(e);
			TrimEventBuffer();
		};
		SetWindowCallbacks(wc);
	}
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
