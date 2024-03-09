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

        glfwSetWindowUserPointer(window, &callbacks);
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


    void GLFWWindow::SetKeyboardCallbacks(const KeyboardCallbacks& kbdCallbacks)
    {
        callbacks.kbdCallbacks = kbdCallbacks;
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCharCallback(window, CharCallback);
    }

    void GLFWWindow::SetMouseCallbacks(const MouseCallbacks& mouseCallbacks)
    {
        callbacks.mouseCallbacks = mouseCallbacks;
        glfwSetCursorPosCallback(window, MousePositionCallback);
        glfwSetCursorEnterCallback(window, CursorEnterCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);
    }

    void GLFWWindow::SetWindowCallbacks(const WindowCallbacks& windowCallbacks)
    {
        callbacks.windowCallbacks = windowCallbacks;
        glfwSetWindowCloseCallback(window, WindowCloseCallback);
        glfwSetWindowSizeCallback(window, WindowSizeCallback);
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
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

    void GLFWWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
        auto& keyboard = callbacks->kbdCallbacks;

        switch (action)
        {
        case GLFW_PRESS:
            if(keyboard.keyPressed)
                keyboard.keyPressed(key, false);
            break;
        case GLFW_REPEAT:
            if(keyboard.keyPressed)
                keyboard.keyPressed(key, true);
            break;
        case GLFW_RELEASE:
            if(keyboard.keyReleased)
                keyboard.keyReleased(key);
            break;
        default:
            assert(false);
            break;
        }
    }

    void GLFWWindow::CharCallback(GLFWwindow* window, unsigned int codepoint)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
        auto& keyboard = callbacks->kbdCallbacks;

        char32_t character = codepoint;
        if(keyboard.characterWrite)
            keyboard.characterWrite(character);
    }

    void GLFWWindow::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
        auto& mouse = callbacks->mouseCallbacks;

        if(mouse.mousePosChange)
            mouse.mousePosChange((int)std::floor(xpos), (int)std::floor(ypos));
    }

    void GLFWWindow::CursorEnterCallback(GLFWwindow* window, int entered)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
        auto& mouse = callbacks->mouseCallbacks;

        if (entered)
        {
            if (mouse.mouseEnterWindow)
                mouse.mouseEnterWindow();
        }
        else
        {
            if (mouse.mouseLeaveWindow)
                mouse.mouseLeaveWindow();
        }
    }

    void GLFWWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
        auto& mouse = callbacks->mouseCallbacks;

        double dx;
        double dy;
        glfwGetCursorPos(window, &dx, &dy);
        int x = (int)std::floor(dx);
        int y = (int)std::floor(dy);

        if (action == GLFW_PRESS)
        {
            if (mouse.mouseButtonPressed)
                mouse.mouseButtonPressed(button, x, y);
        }
        else
        {
            if (mouse.mouseButtonReleased)
                mouse.mouseButtonReleased(button, x, y);
        }
    }

    void GLFWWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(window));
        auto& mouse = callbacks->mouseCallbacks;

        double dx;
        double dy;
        glfwGetCursorPos(window, &dx, &dy);
        int x = (int)std::floor(dx);
        int y = (int)std::floor(dy);

        if(mouse.mouseWheelDelta)
            mouse.mouseWheelDelta((int)std::floor(yoffset), x, y);
    }

    void GLFWWindow::WindowCloseCallback(GLFWwindow* glfwWindow)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(glfwWindow));
        auto& window = callbacks->windowCallbacks;

        if (window.windowShouldClose)
            window.windowShouldClose();
    }
    void GLFWWindow::WindowSizeCallback(GLFWwindow* glfwWindow, int width, int height)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(glfwWindow));
        auto& window = callbacks->windowCallbacks;

        if (window.windowSizeChanged)
            window.windowSizeChanged(width, height);
    }
    void GLFWWindow::FramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height)
    {
        const auto* callbacks = static_cast<Callbacks*>(glfwGetWindowUserPointer(glfwWindow));
        auto& window = callbacks->windowCallbacks;

        if (window.framebufferSizeChanged)
            window.framebufferSizeChanged(width, height);
    }
}