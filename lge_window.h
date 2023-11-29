#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

// little game engine
namespace lge {
    class LgeWindow {
    public:
        LgeWindow(std::string windowName, int width, int height);
        ~LgeWindow();

        LgeWindow(const LgeWindow &)= delete;
        LgeWindow &operator=(const LgeWindow &) = delete;

        bool shouldClose() {return glfwWindowShouldClose(window);}

    private:
        void initWindow();

        const int width;
        const int height;
        std::string windowName;

        GLFWwindow *window;
    };
} // namespace lge
