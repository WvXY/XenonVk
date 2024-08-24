#pragma once

#include <functional>
#include <vector>

#include "global_alias.hpp"

namespace xev_geo {

struct Box{
  vec3 min;
  vec3 max;
};

struct Sphere{
  vec3 center;
  float radius;
};


} // namespace xev_geo
