#pragma once

#include "../renderer/vk_model.hpp"
#include "physics.hpp"

// #include <glm/gtc/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace xev {
struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 rotation{};

  float& yaw() { return rotation.y; }
  float& pitch() { return rotation.x; }

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
};

struct PointLightComponent {
  float intensity{1.0f};
};

class XevGameObject {
public:
  using id_t = unsigned int;
  using Map  = std::unordered_map<id_t, XevGameObject>;

  static XevGameObject createGameObject() {
    static id_t currentId = 0;
    return XevGameObject{currentId++};
  }

  static XevGameObject makePointLight(
      float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3{1.0f});

  XevGameObject(const XevGameObject&) = delete;
  XevGameObject& operator=(const XevGameObject&) = delete;
  XevGameObject(XevGameObject&&) = default;
  XevGameObject& operator=(XevGameObject&&) = default;

  id_t getId() const { return id; }

  glm::vec3 color{};

  std::shared_ptr<XevModel> model{};
  std::unique_ptr<PointLightComponent> pointLight = nullptr;
  TransformComponent transform{};
  MotionComponent motion{transform.translation};

  void update(float dt) {
    motion.setPos(transform.translation);
    motion.update(dt);
    transform.translation = motion.getPosition();
  }

private:
  id_t id{};

  explicit XevGameObject(id_t objId) : id{objId} {}
};
} // namespace xev
