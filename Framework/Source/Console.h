#pragma once

namespace Framework {
	class Console {
	public:
		using MessageSendCallback = std::function<void(const std::string&)>;
		virtual ~Console() = default;

		virtual void AddMessage(const std::string& message) = 0;

		void SetMessageSendCallback(const MessageSendCallback& callback);

	protected:
		MessageSendCallback messageSendCallback;
	};
}