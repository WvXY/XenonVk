#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <string>

namespace xev {
class XevWindow {
public:
  XevWindow(std::string windowName, int width, int height);
  ~XevWindow();
  XevWindow(const XevWindow&)            = delete;
  XevWindow& operator=(const XevWindow&) = delete;

  bool shouldClose() { return glfwWindowShouldClose(window); }

  GLFWwindow* getGLFWwindow() { return window; }
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  bool wasWindowResized() { return framebufferResized; }
  bool wasMouseMoved() { return mouseMoved; }
  bool wasMouseScrolled() { return mouseScrolled; }
  void resetWindowResizedFlag() { framebufferResized = false; }
  void resetMouseMovedFlag() {
    mouseMoved = false;
    mouseDelta = {0.f, 0.f};
  }
  void resetMouseScrolledFlag() {
    mouseScrolled = false;
    scrollDelta   = 0.f;
  }
  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

  glm::vec2 getMousePos() { return mousePos; }
  glm::vec2 getMouseDelta() { return mouseDelta; }
  glm::vec2 getMouseAccumDelta() {
    glm::vec2 delta = mouseAccumDelta;
    mouseAccumDelta = {0.f, 0.f};
    return delta;
  }
  float getScrollDelta() { return scrollDelta; }

  void addInfoToTitle(const std::string& info);

private:
  static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
  static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

  int width;
  int height;
  void initWindow();
  GLFWwindow* window{};
  std::string windowName;

  glm::vec2 mousePos{0.f};
  glm::vec2 mouseDelta{0.f};
  glm::vec2 mouseAccumDelta{0.f};
  float scrollDelta{0.f};
  bool mouseMoved         = false;
  bool mouseScrolled      = false;
  bool framebufferResized = false;
};
} // namespace xev
