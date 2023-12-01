#include "lge_window.h"

#include <utility>

#include <stdexcept>

namespace lge {
    LgeWindow::LgeWindow(std::string name, int w, int h)
            : windowName{std::move(name)}, width{w}, height{h} { initWindow(); }

    LgeWindow::~LgeWindow() {
      glfwDestroyWindow(window);
      glfwTerminate();
    }

    void LgeWindow::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

    void LgeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
      if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface.");
      }
    }
}