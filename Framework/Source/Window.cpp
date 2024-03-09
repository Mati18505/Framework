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
		WindowCallbacks wc;
		wc.focusChanged = [&](char32_t character) {
			kbd.ClearState();
		};
		SetKeyboardCallbacks(kc);
	}
}
