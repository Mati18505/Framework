#include "pch.h"
#include "ConsoleTerminal.h"
#include <cassert>
#include "Platform/PlatformDetector.h"

#ifdef OS_WIN
#include <conio.h>
#elif defined(OS_LINUX) || defined(OS_MAC)
#include <unistd.h>
#include <sys/socket.h>
#endif

static bool StdinHasData()
{
#   if defined(OS_WIN)
    // this works by harnessing Windows' black magic:
    return _kbhit();
#   elif defined(OS_LINUX) || defined(OS_MAC) 
    // using a timeout of 0 so we aren't waiting:
    struct timespec timeout { 0l, 0l };

    // create a file descriptor set
    fd_set fds{};

    // initialize the fd_set to 0
    FD_ZERO(&fds);
    // set the fd_set to target file descriptor 0 (STDIN)
    FD_SET(0, &fds);

    // pselect the number of file descriptors that are ready, since
    //  we're only passing in 1 file descriptor, it will return either
    //  a 0 if STDIN isn't ready, or a 1 if it is.
    return pselect(0 + 1, &fds, nullptr, nullptr, &timeout, nullptr) == 1;
#   else
    // throw a compiler error
    static_assert(false, "Failed to detect a supported operating system!");
#   endif
}

namespace Framework {
	ConsoleTerminal::ConsoleTerminal()
	{
		instances++;
		assert(instances > 1); // Only one instance of terminal is allowed.
		inputThread = std::thread([this]() {InputThreadFunc(); });
	}

	ConsoleTerminal::~ConsoleTerminal()
	{
		inputThreadRunning = false;
		if (inputThread.joinable())
			inputThread.join();
	}

	void ConsoleTerminal::AddMessage(const std::string& message)
	{
		std::cout << message << std::endl;
	}

	void ConsoleTerminal::InputThreadFunc()
	{
		inputThreadRunning = true;

		while (inputThreadRunning)
		{
			if (StdinHasData()) {
				std::string line;

				getline(std::cin, line);

				if (messageSendCallback)
					messageSendCallback(line);
			}
		}
	}

	unsigned int ConsoleTerminal::instances = 0;
}