#include "xev_controller.hpp"

namespace xev {

void XevController::moveInPlaneXZ(
    GLFWwindow* window, float dt, XevGameObject& gameObject) {
  glm::vec3 rotate{0};
  if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) { rotate.x += 1.f; }
  if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) { rotate.x -= 1.f; }
  if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) { rotate.y += 1.f; }
  if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) { rotate.y -= 1.f; }

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
  }

  gameObject.transform.rotation.x =
      glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
  gameObject.transform.rotation.y =
      glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

  float yaw = gameObject.transform.rotation.y;
  const glm::vec3 forwardDir{glm::sin(yaw), 0.f, glm::cos(yaw)};
  const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
  const glm::vec3 upDir{0.f, -1.f, 0.f};

  glm::vec3 moveDir{0.f};
  if (glfwGetKey(window, keys.forward) == GLFW_PRESS) { moveDir += forwardDir; }
  if (glfwGetKey(window, keys.back) == GLFW_PRESS) { moveDir -= forwardDir; }
  if (glfwGetKey(window, keys.right) == GLFW_PRESS) { moveDir += rightDir; }
  if (glfwGetKey(window, keys.left) == GLFW_PRESS) { moveDir -= rightDir; }
  if (glfwGetKey(window, keys.up) == GLFW_PRESS) { moveDir += upDir; }
  if (glfwGetKey(window, keys.down) == GLFW_PRESS) { moveDir -= upDir; }

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
  }
}
void XevController::mouseLook(
    GLFWwindow* window, float dt, float xoffset, float yoffset, XevGameObject& gameObject,
    bool constrainPitch) {
  if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) { return; }

  glm::vec3 rotate{-yoffset, xoffset, 0.f};
  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.rotation += mouseSensitivity * dt * glm::normalize(rotate);
  }

  if (constrainPitch)
    gameObject.transform.rotation.x =
        glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);

  gameObject.transform.rotation.y =
      glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
}

} // namespace xev
