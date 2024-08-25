#pragma once

#include <utility>
#include <vector>

#include "global_alias.hpp"

namespace xev {

class Particle {
public:
  vec2 position{0.f};
  vec2 velocity{0.f};
  vec2 acceleration{0.f};
  vec2 forceAccum{0.f};

  Particle() = default;
  explicit Particle(const vec2& pos) : position(pos) {}

  float_t damping;
  float_t invMass;

  void integrate(float_t duration) {
    assert(duration > 0.f);
    position += velocity * duration;
    vec2 resultingAcc = forceAccum * invMass;
    velocity += resultingAcc * duration;

    velocity *= pow(damping, duration);
  }
};

} // namespace xev