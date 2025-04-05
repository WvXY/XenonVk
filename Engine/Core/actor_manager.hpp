#pragma once

#include "actor.hpp"
#include "entity_manager.hpp"

namespace xev {

// TODO: rename to ActorManager (shorter name)
class ActorManager {
public:
  explicit ActorManager(EntityManager& entityManager) : entityManager(entityManager) {}

  // Create a new game object and add it to the map
  Actor createActor() {
    Entity entityId = entityManager.createEntity();
    Actor actor(entityId);
    actors[entityId] = actor;
    return actor;
  }

  // Access game object by Entity
  Actor& getActor(Entity entityId) { return actors[entityId]; }

  std::unordered_map<Entity, Actor> getActors() { return actors; }

private:
  EntityManager& entityManager;
  std::unordered_map<Entity, Actor> actors;
};

} // namespace xev