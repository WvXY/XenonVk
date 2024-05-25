#include "vk_window.hpp"

#include <stdexcept>
#include <utility>

namespace xev {
XevWindow::XevWindow(std::string name, int w, int h)
    : windowName{std::move(name)}, width{w}, height{h} {
  initWindow();
}

XevWindow::~XevWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void XevWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void XevWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface.");
  }
}

void XevWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto xevWindow = reinterpret_cast<XevWindow*>(glfwGetWindowUserPointer(window));
  xevWindow->framebufferResized = true;
  xevWindow->width              = width;
  xevWindow->height             = height;
}
} // namespace xev
