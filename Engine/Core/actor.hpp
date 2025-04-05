#pragma once

#include "entity_manager.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace xev {

// A game object representing an entity with various components
class Actor {
public:
  Actor() = default;
  Actor(Entity entityId) : entityId(entityId) {}
  Actor(const Actor&) = default;
  ~Actor()            = default;

  // Access components via EntityManager
  template <typename T> T& getComponent(EntityManager& entityManager) {
    return entityManager.getComponent<T>(entityId);
  }

  template <typename T> bool hasComponent(EntityManager& entityManager) {
    return entityManager.hasComponent<T>(entityId);
  }

  Entity getEntityId() const { return entityId; }

private:
  Entity entityId;
};

} // namespace xev