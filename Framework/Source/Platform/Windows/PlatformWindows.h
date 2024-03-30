#pragma once
#include "Platform/Platform.h"

namespace Framework {
	class PlatformWindows : public PlatformImpl {
	public:
		PlatformWindows();
		~PlatformWindows() override;
	};
}

