#include "pch.h"
#include "PlatformWindows.h"
#include "GLFWWindow.h"
#include "DX11Graphics.h"
#include "DX11Global.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#undef CreateWindow

namespace Framework {


	PlatformWindows::PlatformWindows()
	{
		GLFWWindow::Initialize();
		DX11Global(); // TODO: refactor this shit.
	}


	PlatformWindows::~PlatformWindows()
	{
		GLFWWindow::Deinitialize();
	}

	std::unique_ptr<Window> PlatformWindows::CreateWindow(const Window::WindowDesc& desc)
	{
		auto wnd = std::make_unique<GLFWWindow>(desc);
		HWND hwnd = glfwGetWin32Window(wnd->GetGLFWwindow());
		wnd->SetGfx(std::make_unique<DX11Graphics>(wnd.get(), hwnd));
		return wnd;
	}

}