#include "first_app.h"

namespace lge {
    void FirstApp::run() {
      while (!lgeWindow.shouldClose()){
        glfwPollEvents();
      }


    }
}
