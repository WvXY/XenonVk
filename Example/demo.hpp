#pragma once

#include <memory>
#include <string>

#include "actor_manager.hpp"
#include "app_interface.hpp"
#include "entity_manager.hpp"
#include "time_manager.hpp"
#include "vk_descriptors.hpp"
#include "vk_device.hpp"
#include "vk_renderer.hpp"
#include "vk_window.hpp"

namespace xev {
class DemoApp : public IApp {
public:
  static constexpr int WIDTH  = 1280;
  static constexpr int HEIGHT = 720;

  DemoApp();
  ~DemoApp();
  DemoApp(const DemoApp&)            = delete;
  DemoApp& operator=(const DemoApp&) = delete;

  void run() override;

private:
  XevWindow xevWindow{"XenonAppDemo", WIDTH, HEIGHT};
  XevDevice xevDevice{xevWindow};
  XevRenderer xevRenderer{xevWindow, xevDevice};
  TimeManager timeManager{};

  EntityManager& entityManager = EntityManager::instance();
  ActorManager actorManager{entityManager};

  void load() override;
  void tick(float dt) override;
  void fixedTick(float& timeLag) override;

  std::unique_ptr<XevDescriptorPool> globalPool{};
  // XevGameObject::Map gameObjects;

  const std::string relativeModelPath = "../../Assets/models/";
};
} // namespace xev
