#include "pch.h"
#include "Platform.h"
#include "PlatformDetector.h"
#include "Windows/PlatformWindows.h"

namespace Framework
{
	void Platform::Init()
	{
#ifdef OS_WIN
		impl = std::make_unique<PlatformWindows>();
#endif
	}

	std::unique_ptr<Window> Platform::CreateWindow(const Window::WindowDesc& desc)
	{
		return impl->CreateWindow(desc);
	}
}

