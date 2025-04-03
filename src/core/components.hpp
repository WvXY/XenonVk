#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "vk_model.hpp"

namespace xev {

// TODO: separate into different files when getting too big

struct ModelComponent {
  std::shared_ptr<XevModel> model;
  std::string modelPath;
};

struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 rotation{};

  float& yaw() { return rotation.y; }
  float& pitch() { return rotation.x; }

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  [[nodiscard]] glm::mat4 getMat4() const;
  [[nodiscard]] glm::mat3 getNormalMat3() const;
};

struct PointLightComponent {
  float intensity{1.0f};
  glm::vec3 color{1.0f, 1.0f, 1.0f};
};

} // namespace xev
