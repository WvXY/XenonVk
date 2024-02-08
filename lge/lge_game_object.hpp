#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "lge_model.hpp"
// #include <glm/gtc/quaternion.hpp>

// std
#include <memory>

namespace lge {
struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 rotation{};

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();
  glm::mat3 normalMatrix();
};

class LgeGameObject {
public:
  using id_t = unsigned int;

  static LgeGameObject createGameObject() {
    static id_t currentId = 0;
    return LgeGameObject{currentId++};
  }

  LgeGameObject(const LgeGameObject&)            = delete;
  LgeGameObject& operator=(const LgeGameObject&) = delete;
  LgeGameObject(LgeGameObject&&)                 = default;
  LgeGameObject& operator=(LgeGameObject&&)      = default;

  //       id_t getId() const { return id; }

  const id_t id{};
  std::shared_ptr<LgeModel> model{};
  glm::vec3 color{};
  TransformComponent transform{};

private:
  explicit LgeGameObject(id_t objId) : id{objId} {}
};
} // namespace lge
