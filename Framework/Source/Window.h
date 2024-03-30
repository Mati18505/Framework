#pragma once
#include "FrameworkException.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <cassert>

namespace Framework {
    class Graphics;
    class Window {
    public:
        class Exception : public FrameworkException
        {
        public:
            Exception(int line, const std::string& file, const std::string& message)
                : FrameworkException(line, file), message(message)
            {}
            std::string GetType() const override { return "Window Exception"; }
            std::string GetMessage() const override { return message; }
        private:
            std::string message;
        };

        virtual ~Window() = default;

        enum class Mode {
            Windowed,
            BorderlessWindow,
            FullScreen
        };

        struct WindowSize {
            int width = 1366;
            int height = 768;
        };

        struct WindowDesc {
            Mode mode = Mode::Windowed;
            WindowSize size;
            std::string title = "Untitled Window";
            bool resizable = true;
            bool decorated = true;
            bool focused = true;
            int fullscreenRefreshRate = -1;
        };

		struct IoCParams
		{
			WindowDesc desc;
		};

        virtual bool ShouldClose() = 0;

        enum class CursorMode
        {
            Normal,
            Disabled
        };

        virtual void SetCursorMode(CursorMode mode) = 0;
        virtual void PollEvents() = 0;

        virtual void SetMode(Mode mode) = 0;
        virtual void SetSize(WindowSize size) = 0;
        virtual WindowSize GetFramebufferSize() = 0;
		virtual void SetTitle(const std::string& title) = 0;

        Keyboard kbd;
        Mouse mouse;
        Graphics& Gfx() const;

        std::function<void(Event& e)> eventHandler;
        void DispatchEvents();

        class Event : public Framework::Event
        {
        public:
            enum class Type
            {
                ShouldClose,
                //In screen coordinates. Not pixels!
                WindowSizeChanged,
                //In pixels
                FramebufferSizeChanged,
                FocusEnter,
                FocusLeave
            };

        private:
            Type type;
            int framebufferWidth;
            int framebufferHeight;

        public:
            Event(Type type)
                :
                type(type),
                framebufferWidth(0),
                framebufferHeight(0)
            {}
            Event(Type type, int framebufferWidth, int framebufferHeight)
                :
                type(type),
                framebufferWidth(framebufferWidth),
                framebufferHeight(framebufferHeight)
            {}

            Type GetType() const
            {
                return type;
            }

            int GetFramebufferWidth() const
            {
                assert(type == Type::FramebufferSizeChanged);
                return framebufferWidth;
            }
            int GetFramebufferHeight() const
            {
                assert(type == Type::FramebufferSizeChanged);
                return framebufferHeight;
            }

            static EventType GetStaticType() { return EventType::window; }
            EventType GetEventType() const override { return GetStaticType(); };
        };

        static std::shared_ptr<Window> Create(const WindowDesc& desc);
    protected:
        std::queue<Window::Event> windowEvents;

        void TrimEventBuffer();

		std::shared_ptr<Graphics> gfx;
    };
}
