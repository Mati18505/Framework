#pragma once
#include "Window.h"

namespace Framework {
	class PlatformImpl;
	class Window;

	class Platform {
	public:
		static void Init();
	private:
		inline static std::unique_ptr<PlatformImpl> impl;
	};

	class PlatformImpl {
	public:
		virtual ~PlatformImpl() = default;
	};
}