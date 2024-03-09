#pragma once
#include <queue>
#include <optional>
#include "Event.h"

namespace Framework {
	class Mouse
	{
		friend class Window;
	public:
		class Event : public Framework::Event
		{
		public:
			enum class Type
			{
				LPress,
				LRelease,
				RPress,
				RRelease,
				WheelUp,
				WheelDown,
				Move,
				Enter,
				Leave,
			};
		private:
			Type type;
			bool leftIsPressed;
			bool rightIsPressed;
			int x;
			int y;
		public:
			Event(Type type, const Mouse& parent)
				:
				type(type),
				leftIsPressed(parent.leftIsPressed),
				rightIsPressed(parent.rightIsPressed),
				x(parent.x),
				y(parent.y)
			{}
			Type GetType() const
			{
				return type;
			}
			std::pair<int, int> GetPos() const
			{
				return{ x,y };
			}
			int GetPosX() const
			{
				return x;
			}
			int GetPosY() const
			{
				return y;
			}
			bool LeftIsPressed() const
			{
				return leftIsPressed;
			}
			bool RightIsPressed() const
			{
				return rightIsPressed;
			}

			static EventType GetStaticType() { return EventType::mouse; }
			EventType GetEventType() const override { return GetStaticType(); };
		};
	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		std::pair<int, int> GetPos() const;
		int GetPosX() const;
		int GetPosY() const;
		bool IsInWindow() const;
		bool LeftIsPressed() const;
		bool RightIsPressed() const;
		std::optional<Mouse::Event> Read();
		bool IsEmpty() const
		{
			return buffer.empty();
		}
		void Flush();
	private:
		void OnMouseMove(int x, int y);
		void OnMouseLeave();
		void OnMouseEnter();
		void OnLeftPressed();
		void OnLeftReleased();
		void OnRightPressed();
		void OnRightReleased();
		void OnWheelUp();
		void OnWheelDown();
		void TrimBuffer();
	private:
		static constexpr unsigned int bufferSize = 16u;
		int x;
		int y;
		bool leftIsPressed = false;
		bool rightIsPressed = false;
		bool isInWindow = false;
		std::queue<Event> buffer;
	};
}