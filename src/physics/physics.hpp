#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../common/global_alias.hpp"
#include "AABB.hpp"

namespace xev {

class MotionComponent {
  // 3D motion component
public:
  MotionComponent() = default;
  MotionComponent(vec3& pos) : pos(pos) {}
  MotionComponent(vec3& pos, vec3& vel) : pos(pos), vel(vel) {}

  MotionComponent operator=(const MotionComponent&) = delete;
  ~MotionComponent()                                = default;

  void update(float dt) {
    // if (atRest()) { return; }

    detectHandleCollision();

    vec3 velPrev = vel;
    vel += computeAcceleration() * dt;
    pos += (vel + velPrev) * 0.5f * dt;
  }

  // setters
  void addForce(const vec3& f) { force += f; }
  void setWind(const vec3& vel) { windVel = vel; }
  void setMass(const float m) { mass_inv = 1.f / m; }
  void setPos(const vec3& p) { pos = p; }
  void setGravityMode(const bool isOn) { isGravityOn = isOn; }
  void setGravity(const float_t new_gravity) { gravity = new_gravity; }
  void setVelocity(const vec3& v) { vel = v; }

  // getters
  vec3 getPosition() const { return pos; }
  vec3 getVelocity() const { return vel; }
  vec3 getForce() const { return force; }

  vec3 computeAcceleration() {
    acc = force * mass_inv + (windVel - vel) * airResistance * mass_inv;
    if (isGravityOn) { acc.y -= gravity; }
    return acc;
  }

private:
  vec3 pos{0.f};
  vec3 vel{0.f};
  vec3 acc{0.f};
  vec3 force{0.f};
  vec3 windVel{0.f};

  float damping       = 0.99f;
  float airResistance = 0.3f;
  float gravity       = 9.8f; // m/s^2
  bool isGravityOn    = true;
  float mass_inv      = 1.0f; // inverse mass

  bool atRest() const {
    // There are 4 conditions for particle to be at rest
    // 1. velocity is nearly zero
    // 2. object touches an object
    // 3. force is zero or direction is opposite to the normal
    // 4. object is out of bounds
    return glm::length(vel) < 0.001f && (abs(pos.y) > 0.99f || abs(pos.x) > 0.99f);
  }

  void detectHandleCollision() {
    if (pos.y < 0.0f) {
      pos.y = 0.0f;
      vel.y = -vel.y * damping;
    }
  }
};
} // namespace xev