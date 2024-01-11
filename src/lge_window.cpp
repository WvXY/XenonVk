#include "lge_window.hpp"

#include <stdexcept>
#include <utility>

namespace lge {
LgeWindow::LgeWindow(std::string name, int w, int h)
    : windowName{std::move(name)}, width{w}, height{h} {
  initWindow();
}

LgeWindow::~LgeWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void LgeWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void LgeWindow::createWindowSurface(
    VkInstance instance, VkSurfaceKHR* surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface.");
  }
}

void LgeWindow::framebufferResizeCallback(
    GLFWwindow* window, int width, int height) {
  auto lgeWindow =
      reinterpret_cast<LgeWindow*>(glfwGetWindowUserPointer(window));
  lgeWindow->framebufferResized = true;
  lgeWindow->width = width;
  lgeWindow->height = height;
}
} // namespace lge
