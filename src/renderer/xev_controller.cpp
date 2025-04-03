#include "xev_controller.hpp"

namespace xev {

void XevController::moveInPlaneXZ(
    GLFWwindow* window, float dt, TransformComponent& viewTransform) {
  glm::vec3 rotate{0};
  if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) { rotate.x += 1.f; }
  if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) { rotate.x -= 1.f; }
  if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) { rotate.y += 1.f; }
  if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) { rotate.y -= 1.f; }

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    viewTransform.rotation += lookSpeed * dt * glm::normalize(rotate);
  }

  viewTransform.rotation.x = glm::clamp(viewTransform.rotation.x, -1.5f, 1.5f);
  viewTransform.rotation.y = glm::mod(viewTransform.rotation.y, glm::two_pi<float>());

  float yaw = viewTransform.rotation.y;
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

  float _moveSpeed;
  if (glfwGetKey(window, keys.speedUp) == GLFW_PRESS)
    _moveSpeed = moveSpeed * speedUpMultiplier;
  else
    _moveSpeed = moveSpeed;

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    viewTransform.translation += _moveSpeed * dt * glm::normalize(moveDir);
  }
}

void XevController::mouseLook(
    GLFWwindow* window, float dt, float xoffset, float yoffset,
    TransformComponent& viewTransform, bool constrainPitch) {
  if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) { return; }

  glm::vec3 rotate{-yoffset, xoffset, 0.f};
  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    viewTransform.rotation += mouseSensitivity * dt * glm::normalize(rotate);
  }

  if (constrainPitch)
    viewTransform.rotation.x = glm::clamp(viewTransform.rotation.x, -1.5f, 1.5f);

  viewTransform.rotation.y = glm::mod(viewTransform.rotation.y, glm::two_pi<float>());
}

} // namespace xev
