#pragma once
#include "FrameworkException.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <cassert>

namespace Framework {
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
        virtual void SetTitle(const std::string& title) = 0;

        struct KeyboardCallbacks {
            std::function<void(int keyCode, bool repeated)> keyPressed;
            std::function<void(int keyCode)> keyReleased;
            std::function<void(char32_t character)> characterWrite;
        };

        struct MouseCallbacks {
            //top_left = 0,0
            std::function<void(int x, int y)> mousePosChange;
            std::function<void()> mouseLeaveWindow;
            std::function<void()> mouseEnterWindow;
            std::function<void(int buttonCode, int x, int y)> mouseButtonPressed;
            std::function<void(int buttonCode, int x, int y)> mouseButtonReleased;
            std::function<void(int delta, int x, int y)> mouseWheelDelta;
        };

        struct WindowCallbacks {
            //Called after ShouldClose returns true
            std::function<void()> windowShouldClose;
            //In screen coordinates. Not pixels!
            std::function<void(int width, int height)> windowSizeChanged;
            //In pixels
            std::function<void(int width, int height)> framebufferSizeChanged;
            std::function<void(bool focused)> focusChanged;
        };

        virtual void SetKeyboardCallbacks(const KeyboardCallbacks& kbdCallbacks) = 0;
        virtual void SetMouseCallbacks(const MouseCallbacks& mouseCallbacks) = 0;
        virtual void SetWindowCallbacks(const WindowCallbacks& windowCallbacks) = 0;
        virtual KeyboardCallbacks GetKeyboardCallbacks() = 0;
        virtual MouseCallbacks GetMouseCallbacks() = 0;
        virtual WindowCallbacks GetWindowCallbacks() = 0;

        Keyboard kbd;
        Mouse mouse;

        void SetDefaultCallbacks();

        std::function<void(Event& e)> eventHandler;
        void DispatchEvents();

        class Event : public Framework::Event
        {
        public:
            enum class Type
            {
                ShouldClose,
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

        std::queue<Window::Event> windowEvents;

        void TrimEventBuffer();
    };
}
