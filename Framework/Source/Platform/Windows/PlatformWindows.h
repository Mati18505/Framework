#pragma once
#include "Platform/Platform.h"

namespace Framework {
	class PlatformWindows : public PlatformImpl {
	public:
		PlatformWindows();
		~PlatformWindows() override;
		std::unique_ptr<Window> CreateWindow(const Window::WindowDesc& desc) override;
	};
}

