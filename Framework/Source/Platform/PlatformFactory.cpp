#include "pch.h"
#include "PlatformFactory.h"
#include "Windows/GLFWWindow.h"

namespace Framework {
	std::unique_ptr<Window> PlatformFactory::CreateWindow(Window::WindowDesc desc)
	{
		return std::make_unique<GLFWWindow>(desc);
	}
}