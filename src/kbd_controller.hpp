#pragma once

#include "lge_game_object.hpp"
#include "lge_window.hpp"

namespace lge {
class KbdController {
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
  };

  void moveInPlaneXZ(GLFWwindow* window, float dt, LgeGameObject& gameObject);

  KeyMappings keys{};
  float moveSpeed{2.0f};
  float lookSpeed{0.5f};
};

} // namespace lge
