#include "lge_window.h"

namespace lge {
    LgeWindow::LgeWindow(std::string name, int w, int h)
            : windowName{name}, width{w}, height{h} { initWindow(); }

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
}