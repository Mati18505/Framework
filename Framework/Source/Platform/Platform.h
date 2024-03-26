#pragma once
#include "Window.h"

namespace Framework {
	class PlatformImpl;
	class Window;

	class Platform {
	public:
		static void Init();
		static std::unique_ptr<Window> CreateWindow(const Window::WindowDesc& desc);
	private:
		inline static std::unique_ptr<PlatformImpl> impl;
	};

	class PlatformImpl {
	public:
		virtual ~PlatformImpl() = default;
		virtual std::unique_ptr<Window> CreateWindow(const Window::WindowDesc& desc) = 0;
	};
}