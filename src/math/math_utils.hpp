#pragma once

#include <complex>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "global_alias.hpp"

#ifndef EPSILON
#define EPSILON 1e-3
#endif

namespace xev {

struct MathUtils {
  static float_t norm(const auto& x) {
    // return glm::sqrt(glm::dot(x, x));
    return glm::length(x);
  }

  static auto normalize(const auto& x) { return glm::normalize(x); }

  static auto cmp_float(const auto& x, const auto& y) {
    return glm::epsilonEqual(x, y, EPSILON);
  }

  static mat2 rotaion2(float_t theta) {
    float_t s = std::sin(theta), c = std::cos(theta);
    return mat2{c, s, -s, c};
  }
};

} // namespace xev