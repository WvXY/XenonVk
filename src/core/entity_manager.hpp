#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "components.hpp"

namespace xev {

using Entity = uint64_t;

class EntityManager {
public:
  static EntityManager& instance() {
    static EntityManager instance;
    return instance;
  } // Singleton pattern, not sure if this is the best way to do it

  // Create a new entity
  Entity createEntity() {
    uint64_t entityId = nextEntityId++;
    return entityId;
  }

  template <typename T> void addComponent(Entity entityId, T component) {
    auto componentType                        = std::type_index(typeid(T));
    componentStorage[entityId][componentType] = std::move(component);
  }

  template <typename T> T& getComponent(Entity entityId) {
    auto componentType = std::type_index(typeid(T));
    return std::any_cast<T&>(componentStorage[entityId][componentType]);
  }

  template <typename T> bool hasComponent(Entity entityId) {
    auto componentType = std::type_index(typeid(T));
    return componentStorage[entityId].count(componentType) > 0;
  }

  template <typename T> void removeComponent(Entity entityId) {
    auto componentType = std::type_index(typeid(T));
    componentStorage[entityId].erase(componentType);
  }

  template <typename... T> std::vector<Entity> getEntitiesWithComponents() {
    std::vector<Entity> entities;

    // Iterate over all entities and check for components
    for (const auto& entityPair : componentStorage) {
      Entity entityId       = entityPair.first;
      bool hasAllComponents = (hasComponent<T>(entityId) && ...);

      if (hasAllComponents) { entities.push_back(entityId); }
    }
    return entities;
  }

private:
  EntityManager()                                = default;
  EntityManager(const EntityManager&)            = delete;
  EntityManager& operator=(const EntityManager&) = delete;

  Entity nextEntityId = 0;

  // Storage for all components of an entity (keyed by entity ID)
  std::unordered_map<Entity, std::unordered_map<std::type_index, std::any>>
      componentStorage;
};

} // namespace xev