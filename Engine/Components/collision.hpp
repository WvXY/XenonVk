#pragma once

#include "AABB.hpp"
#include "xev_alias.hpp"

namespace xev {

class CollisionComponent {
public:
  AABB<vec2> aabb;
};

} // namespace xev
