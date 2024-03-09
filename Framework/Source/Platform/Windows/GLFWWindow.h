#pragma once
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Framework
{
    static int windowCount = 0;
    class GLFWWindow : public Window {
    public:
        explicit GLFWWindow(const WindowDesc& desc);
        ~GLFWWindow() override;

        bool ShouldClose() override;

        void SetCursorMode(CursorMode mode) override;

        void PollEvents() override;

        void SetMode(Mode mode) override;

        void SetSize(WindowSize size) override;

        void SetTitle(const std::string& title) override;

        struct Callbacks
        {
            KeyboardCallbacks kbdCallbacks;
            MouseCallbacks mouseCallbacks;
            WindowCallbacks windowCallbacks;
        };

        void SetKeyboardCallbacks(const KeyboardCallbacks& kbdCallbacks) override;
        void SetMouseCallbacks(const MouseCallbacks& mouseCallbacks) override;
        void SetWindowCallbacks(const WindowCallbacks& windowCallbacks) override;
        KeyboardCallbacks GetKeyboardCallbacks() override;
        MouseCallbacks GetMouseCallbacks() override;
        WindowCallbacks GetWindowCallbacks() override;

        GLFWwindow* GetGLFWwindow() { return window; }

    private:
        GLFWwindow* window;
        WindowDesc desc;
        Callbacks callbacks;

        static void GLFWErrorCallback(int error, const char* description);

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CharCallback(GLFWwindow* window, unsigned int codepoint);
        static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void CursorEnterCallback(GLFWwindow* window, int entered);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void WindowCloseCallback(GLFWwindow* window);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void FocusCallback(GLFWwindow* glfwWindow, int focused);

        void Initialize();
        void Deinitialize();
    };
}