#include "pch.h"
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>
#include <format>
#include <cassert>

#define WND_EXCEPTION(message) Window::Exception(__LINE__, __FILE__, message)

namespace Framework
{
    GLFWWindow::GLFWWindow(const WindowDesc& desc)
    {
        if (windowCount == 0)
            Initialize();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, desc.resizable);
        glfwWindowHint(GLFW_DECORATED, desc.decorated);
        glfwWindowHint(GLFW_FOCUSED, desc.focused);
        glfwWindowHint(GLFW_REFRESH_RATE, desc.fullscreenRefreshRate);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);

        GLFWmonitor* windowMonitor = nullptr;

        switch (desc.mode)
        {
        case Mode::Windowed:
            break;
        case Mode::BorderlessWindow:
            break;
        case Mode::FullScreen:
            windowMonitor = glfwGetPrimaryMonitor();
            break;
        default:
            break;
        }

        window = glfwCreateWindow(desc.size.width, desc.size.height, desc.title.c_str(), windowMonitor, nullptr);
        
        if (window == nullptr) {
            throw WND_EXCEPTION("GLFW window creation failed!");
        }
        windowCount++;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glfwSetWindowUserPointer(window, this);
        SetCallbacks();
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(window);
        windowCount--;
        if (windowCount == 0)
            Deinitialize();
    }
    
    bool GLFWWindow::ShouldClose() {
        return glfwWindowShouldClose(window);
    }
    void GLFWWindow::SetCursorMode(CursorMode mode)
    {
        switch (mode)
        {
        case CursorMode::Normal:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CursorMode::Disabled:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }

    }
    void GLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFWWindow::SetMode(Mode mode)
    {
        GLFWmonitor* windowMonitor = nullptr;
        WindowSize size;

        switch (mode)
        {
        case Mode::Windowed:
            break;
        case Mode::BorderlessWindow:
            windowMonitor = glfwGetPrimaryMonitor();
            break;
        case Mode::FullScreen:
            windowMonitor = glfwGetPrimaryMonitor();
            break;
        default:
            break;
        }

        if (mode == Mode::FullScreen || mode == Mode::BorderlessWindow)
        {
            const GLFWvidmode* vidmode = glfwGetVideoMode(windowMonitor);
            size = { vidmode->width, vidmode->height };
        }
        else
            size = desc.size;

        glfwSetWindowMonitor(window, windowMonitor, 0, 0, size.width, size.height, GLFW_DONT_CARE);
    }

    void GLFWWindow::SetSize(WindowSize size)
    {
        glfwSetWindowSize(window, size.width, size.height);
    }

    void GLFWWindow::SetTitle(const std::string& title)
    {
        glfwSetWindowTitle(window, title.c_str());
    }

    void GLFWWindow::GLFWErrorCallback(int error, const char* description)
    {
        throw WND_EXCEPTION(std::format("GLFW Error: ({}): {}", error, description));
    }

    void GLFWWindow::SetCallbacks()
    {
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCharCallback(window, CharCallback);

        glfwSetCursorPosCallback(window, MousePositionCallback);
        glfwSetCursorEnterCallback(window, CursorEnterCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);

        glfwSetWindowCloseCallback(window, WindowCloseCallback);
        glfwSetWindowSizeCallback(window, WindowSizeCallback);
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
        glfwSetWindowFocusCallback(window, FocusCallback);
    }

    void GLFWWindow::MakeCurrent()
    {
        CreateFn = CreateCurrent;
    }

    void GLFWWindow::Initialize()
    {
        if (!glfwInit()) {
            throw WND_EXCEPTION("GLFW init failed!");
        }
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    void GLFWWindow::Deinitialize()
    {
        glfwTerminate();
    }

    std::unique_ptr<Window> GLFWWindow::CreateCurrent(WindowDesc desc)
    {
        return std::make_unique<GLFWWindow>(desc);
    }

    void GLFWWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
	void GLFWWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_UNKNOWN)
			return;
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        auto& kbd = userPtr->kbd;

        switch (action)
        {
        case GLFW_PRESS:
            kbd.OnKeyPressed(key, false);
            break;
        case GLFW_REPEAT:
            if (!kbd.AutorepeatIsEnabled())
                return;
            kbd.OnKeyPressed(key, true);
            break;
        case GLFW_RELEASE:
            kbd.OnKeyReleased(key);
            break;
        default:
            assert(false);
            break;
        }
    }

    void GLFWWindow::CharCallback(GLFWwindow* window, unsigned int codepoint)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        auto& kbd = userPtr->kbd;

        char32_t character = codepoint;
        kbd.OnCharacterWrite(character);
    }

    void GLFWWindow::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        auto& mouse = userPtr->mouse;

        mouse.OnMouseMove((int)std::floor(xpos), (int)std::floor(ypos));
    }

    void GLFWWindow::CursorEnterCallback(GLFWwindow* window, int entered)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        auto& mouse = userPtr->mouse;

        if (entered)
            mouse.OnMouseEnter();
        else
            mouse.OnMouseLeave();
    }

    void GLFWWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        auto& mouse = userPtr->mouse;

        double dx;
        double dy;
        glfwGetCursorPos(window, &dx, &dy);
        int x = (int)std::floor(dx);
        int y = (int)std::floor(dy);

        if (action == GLFW_PRESS)
        {
            switch (button)
            {
            case 0:
                mouse.OnLeftPressed();
                break;
            case 1:
                mouse.OnRightPressed();
                break;
            default:
                break;
            }
        }
        else
        {
            switch (button)
            {
            case 0:
                mouse.OnLeftReleased();
                break;
            case 1:
                mouse.OnRightReleased();
                break;
            default:
                break;
            }
        }
    }

    void GLFWWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        auto& mouse = userPtr->mouse;

        double dx;
        double dy;
        glfwGetCursorPos(window, &dx, &dy);
        int x = (int)std::floor(dx);
        int y = (int)std::floor(dy);

        int delta = (int)std::floor(yoffset);

        if (delta < 0)
            mouse.OnWheelDown();
        else if (delta > 0)
            mouse.OnWheelUp();
    }

    void GLFWWindow::WindowCloseCallback(GLFWwindow* glfwWindow)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
        auto& win = *userPtr;

        Window::Event e(Window::Event::Type::ShouldClose);
        win.windowEvents.push(e);
        win.TrimEventBuffer();
    }
    void GLFWWindow::WindowSizeCallback(GLFWwindow* glfwWindow, int width, int height)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
        auto& win = *userPtr;

        Window::Event e(Window::Event::Type::WindowSizeChanged, width, height);
        win.windowEvents.push(e);
        win.TrimEventBuffer();
    }
    void GLFWWindow::FramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
        auto& win = *userPtr;

        Window::Event e(Window::Event::Type::FramebufferSizeChanged, width, height);
        win.windowEvents.push(e);
        win.TrimEventBuffer();
    }
    
    void GLFWWindow::FocusCallback(GLFWwindow* glfwWindow, int focused)
    {
        auto* userPtr = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
        auto& win = *userPtr;

        win.kbd.ClearState();
        if (focused)
        {
            Window::Event e(Window::Event::Type::FocusEnter);
            win.windowEvents.push(e);
            win.TrimEventBuffer();
        }
        else
        {
            Window::Event e(Window::Event::Type::FocusLeave);
            win.windowEvents.push(e);
            win.TrimEventBuffer();
        }
    }
}