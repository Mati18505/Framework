#pragma once
#include <memory>

namespace Framework {
	class Application {
	public:
		void Run();
	};
	std::unique_ptr<Application> CreateApplication();
}
