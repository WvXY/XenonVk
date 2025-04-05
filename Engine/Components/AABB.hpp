#pragma once

#include "actor.hpp"
#include "xev_alias.hpp"
#include <glm/glm.hpp>

namespace xev {

template <typename T> struct AABB {
  AABB() = default;
  AABB(const T& min, const T& max) : min(min), max(max) {}

  T min{};
  T max{};

  AABB operator+(const AABB& other) const {
    AABB result;
    result.max = glm::max(this->max, other.max);
    result.min = glm::min(this->min, other.min);
    return result;
  }

  bool collidesWith(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y);
  }

  static bool hasCollision(const AABB& a, const AABB& b) { return a.collidesWith(b); }
};

} // namespace xev