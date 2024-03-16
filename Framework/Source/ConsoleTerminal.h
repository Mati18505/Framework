#pragma once
#include "Console.h"

namespace Framework {
	class ConsoleTerminal : public Console {
	public:
		ConsoleTerminal();
		~ConsoleTerminal();
		void AddMessage(const std::string& message) override;

	private:
		void InputThreadFunc();

		std::thread inputThread;
		bool inputThreadRunning = false;

		static unsigned int instances;
	};
}