#pragma once
#include "Window.h"

namespace Framework {
	class PlatformFactory {
	public:
		static std::unique_ptr<Window> CreateWindow(Window::WindowDesc desc);
	};
}