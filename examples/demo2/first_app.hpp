#pragma once

#include "lge_device.hpp"
#include "lge_game_object.hpp"
#include "lge_renderer.hpp"
#include "lge_window.hpp"

#include <memory>
#include <string>
#include <vector>

namespace lge2 {
class FirstApp {
public:
  static constexpr int WIDTH  = 1280;
  static constexpr int HEIGHT = 720;

  FirstApp();
  ~FirstApp();
  FirstApp(const FirstApp&)            = delete;
  FirstApp& operator=(const FirstApp&) = delete;

  void run();

private:
  LgeWindow lgeWindow{"Vulkan2D demo", WIDTH, HEIGHT};
  LgeDevice lgeDevice{lgeWindow};
  LgeRenderer lgeRenderer{lgeWindow, lgeDevice};

  void loadGameObjects();
  std::vector<LgeGameObject> gameObjects;
};
} // namespace lge
