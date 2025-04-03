#pragma once

#include "entity_manager.hpp"
#include "vk_game_object.hpp"

namespace xev {

// TODO: rename to ActorManager (shorter name)
class GameObjectManager {
public:
  explicit GameObjectManager(EntityManager& entityManager)
      : entityManager(entityManager) {}

  // Create a new game object and add it to the map
  XevGameObject createGameObject() {
    Entity entityId = entityManager.createEntity();
    XevGameObject gameObject(entityId);
    gameObjects[entityId] = gameObject;
    return gameObject;
  }

  // Access game object by Entity
  XevGameObject& getGameObject(Entity entityId) { return gameObjects[entityId]; }

  std::unordered_map<Entity, XevGameObject> getGameObjects() { return gameObjects; }

private:
  EntityManager& entityManager;
  std::unordered_map<Entity, XevGameObject> gameObjects;
};

} // namespace xev