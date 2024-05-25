#pragma once

#include <functional>
#include <vector>

#include <glm/glm.hpp>

namespace phyx {

class NewtonianPhysics{
public:
  glm::vec2 position{};
  glm::vec2 velocity{};
  glm::vec2 acceleration{};
  float mass{1.0f};

  void inverseMass(float inverseMass){
    mass = 1.0f / inverseMass;
  }
};


}

