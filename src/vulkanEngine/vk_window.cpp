#include "vk_window.hpp"

#include <iostream>
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
  // mouse input
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
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

void XevWindow::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  auto xevWindow        = reinterpret_cast<XevWindow*>(glfwGetWindowUserPointer(window));
  xevWindow->mouseMoved = true;
  xevWindow->mouseDelta = {xpos - xevWindow->mousePos.x, ypos - xevWindow->mousePos.y};
  xevWindow->mousePos   = {xpos, ypos};
}

void XevWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  auto xevWindow = reinterpret_cast<XevWindow*>(glfwGetWindowUserPointer(window));
  xevWindow->mouseScrolled = true;
  xevWindow->scrollDelta   = yoffset;
}
} // namespace xev
