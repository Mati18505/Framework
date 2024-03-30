#include "pch.h"
#include "PlatformWindows.h"
#include "GLFWWindow.h"
#include "DX11Graphics.h"
#include "DX11Global.h"
#include "ioc/Container.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Framework {
	PlatformWindows::PlatformWindows()
	{
		GLFWWindow::Initialize();
		DX11Global(); // TODO: refactor this shit.

		ioc::Container::Get().RegisterFactory<Window>([](Window::IoCParams params) {
			return std::make_shared<GLFWWindow>(std::move(params));
		});

		ioc::Container::Get().RegisterFactory<Graphics>([] (Graphics::IoCParams params){
			HWND hwnd = glfwGetWin32Window(static_cast<GLFWWindow*>(params.window)->GetGLFWwindow());
			return std::make_shared<DX11Graphics>(std::move(params), hwnd);
		});
	}

	PlatformWindows::~PlatformWindows()
	{
		GLFWWindow::Deinitialize();
	}

}