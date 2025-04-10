#pragma once
#include <queue>
#include <bitset>
#include "Event.h"

namespace Framework {
	class Keyboard
	{
		using KeyCode = int;
		friend class Window;
		friend class GLFWWindow;
	public:
		class Event : public Framework::Event
		{
		public:
			enum class Type
			{
				Press,
				Release,
			};
		private:
			Type type;
			KeyCode code;
		public:
			Event(Type type, KeyCode code)
				:
				type(type),
				code(code)
			{}
			bool IsPress() const
			{
				return type == Type::Press;
			}
			bool IsRelease() const
			{
				return type == Type::Release;
			}
			KeyCode GetCode() const
			{
				return code;
			}

			static EventType GetStaticType() { return EventType::keyboard; }
			EventType GetEventType() const override { return GetStaticType(); };
		};

		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		// key event
		bool IsKeyPressed(KeyCode keycode) const;
		std::optional<Event> ReadKey();
		bool KeyIsEmpty() const;
		void FlushKey();
		// char event
		std::optional<char> ReadChar();
		bool CharIsEmpty() const;
		void FlushChar();
		void Flush();
		// autorepeat control
		void EnableAutorepeat();
		void DisableAutorepeat();
		bool AutorepeatIsEnabled() const;
	private:
		void OnKeyPressed(KeyCode keyCode, bool repeated);
		void OnKeyReleased(KeyCode keyCode);
		void OnCharacterWrite(char character);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);

		static constexpr unsigned int nKeys = 350u;
		static constexpr unsigned int bufferSize = 16u;
		bool autorepeatEnabled = false;
		std::bitset<nKeys> keystates;
		std::queue<Event> keybuffer;
		std::queue<char> charbuffer;
	};
}