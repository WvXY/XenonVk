#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "../lge3d/lge_game_object.hpp"

namespace lge {

class AABB {
public:
  AABB() = default;
  AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
  AABB(const lge::LgeGameObject& gameObject) {
    min = gameObject.transform.translation - gameObject.transform.scale;
    max = gameObject.transform.translation + gameObject.transform.scale;
  }

  glm::vec3 min;
  glm::vec3 max;
};


} // namespace lgeBB
