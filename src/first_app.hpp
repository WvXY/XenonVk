#pragma once

#include <memory>
#include <string>

#include "vk_descriptors.hpp"
#include "vk_device.hpp"
#include "vk_renderer.hpp"
#include "vk_window.hpp"
#include "time_manager.hpp"
#include "entity_manager.hpp"
#include "game_object_manager.hpp"


namespace xev {
  class FirstApp {
  public:
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;

    FirstApp();
    ~FirstApp();
    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

  private:
    XevWindow xevWindow{ "XenonAppDemo", WIDTH, HEIGHT };
    XevDevice xevDevice{ xevWindow };
    XevRenderer xevRenderer{ xevWindow, xevDevice };
    TimeManager timeManager{};

    EntityManager& entityManager = EntityManager::instance();
    GameObjectManager gameObjectManager{ entityManager };

    void loadGameObjects();
    void updateGameObjects(float dt);
    void fixedUpdateGameObjects(float& timeLag);

    std::unique_ptr<XevDescriptorPool> globalPool{};
    // XevGameObject::Map gameObjects;

    const std::string relativeModelPath = "../../assets/models/";
  };
} // namespace xev
