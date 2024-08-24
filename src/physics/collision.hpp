#pragma once

#include "AABB.hpp"
#include "xev_aliases.hpp"

namespace xev {

class CollisionComponent {
public:
  AABB<vec2> aabb;

};

}
