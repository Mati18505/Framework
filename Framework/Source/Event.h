#pragma once

namespace Framework {
	enum class EventType {
		mouse, keyboard, window, custom
	};

	class Event {
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		bool handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e)
			: e(e)
		{}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (e.GetEventType() == T::GetStaticType())
			{
				e.handled |= func(static_cast<T&>(e));
				return true;
			}
			return false;
		}
	private:
		Event& e;
	};

	class CustomEvent : public Event
	{
	public:
		CustomEvent(std::string eventName)
			: eventName(eventName)
		{}
		
		std::string GetEventName() const {
			return eventName;
		}

		static EventType GetStaticType() { return EventType::custom; }
		EventType GetEventType() const override { return GetStaticType(); };

	private:
		std::string eventName;
	};
}