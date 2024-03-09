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
		wc.focusChanged = [&](char32_t character) {
			kbd.ClearState();
		};
		SetWindowCallbacks(wc);
	}
}
