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
  LgeWindow(const LgeWindow&) = delete;
  LgeWindow& operator=(const LgeWindow&) = delete;

  bool shouldClose() { return glfwWindowShouldClose(window); }
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }
  bool wasWindowResized() { return framebufferResized; }
  void resetWindowResizedFlag() { framebufferResized = false; }
  GLFWwindow* getGLFWwindow() { return window; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:
  static void
  framebufferResizeCallback(GLFWwindow* window, int width, int height);
  void initWindow();

  int width;
  int height;
  bool framebufferResized = false;
  std::string windowName;

  GLFWwindow* window{};
};
} // namespace lge
