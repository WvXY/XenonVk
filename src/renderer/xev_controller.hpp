#pragma once

#include "vk_game_object.hpp"
#include "vk_window.hpp"

namespace xev {
class XevController { // handle mouse and keyboard input
public:
  struct KeyMappings {
    int left      = GLFW_KEY_A;
    int right     = GLFW_KEY_D;
    int forward   = GLFW_KEY_W;
    int back      = GLFW_KEY_S;
    int up        = GLFW_KEY_Q;
    int down      = GLFW_KEY_E;
    int lookUp    = GLFW_KEY_UP;
    int lookDown  = GLFW_KEY_DOWN;
    int lookLeft  = GLFW_KEY_LEFT;
    int lookRight = GLFW_KEY_RIGHT;
    int rollLeft  = GLFW_KEY_Z;
    int rollRight = GLFW_KEY_C;
    int speedUp   = GLFW_KEY_LEFT_SHIFT;

    int endApp  = GLFW_KEY_ESCAPE;
  };

  void moveInPlaneXZ(GLFWwindow* window, float dt, XevGameObject& gameObject);
  void mouseLook(
      GLFWwindow* window, float dt, float xoffset, float yoffset,
      XevGameObject& gameObject, bool constrainPitch = true);

  void setFov(float fov) { fov = fov; }

  bool isPressed(GLFWwindow* window, int key) {return glfwGetKey(window, key) == GLFW_PRESS;}

  KeyMappings keys{};

  // void processInputs(GLFWwindow* window, float dt, XevGameObject& gameObject) {
  //   moveInPlaneXZ(window, dt, gameObject);
  // }

  // not sure if I put this here
  // glm::vec2 getMouseDelta(glm::vec2 currentMousePos) {
  //   mouseDelta   = currentMousePos - lastMousePos;
  //   lastMousePos = currentMousePos;
  //   return mouseDelta;
  // }

private:
  float mouseSensitivity{6.f};
  float moveSpeed{8.f};
  float lookSpeed{1.6f};
  float speedUpMultiplier{2.f};

  // glm::vec2 mousePos{0.f};
  // glm::vec2 mouseDelta{0.f};
  // glm::vec2 lastMousePos{0.f};
};

} // namespace xev
