#include "pch.h"
#include "WindowsOSInitializer.h"
#include "GLFWWindow.h"

namespace Framework {

	void WindowsOSInitializer::Init()
	{
		GLFWWindow::MakeCurrent();
	}

}