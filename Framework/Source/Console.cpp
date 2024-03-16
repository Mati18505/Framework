#include "pch.h"
#include "Console.h"

namespace Framework {
	void Console::SetMessageSendCallback(const MessageSendCallback& callback)
	{
		messageSendCallback = callback;
	}
}