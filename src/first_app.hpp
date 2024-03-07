#pragma once

#include <memory>
#include <string>
#include <vector>

#include "lge_descriptors.hpp"
#include "lge_device.hpp"
#include "lge_game_object.hpp"
#include "lge_renderer.hpp"
#include "lge_window.hpp"

namespace lge {
class FirstApp {
public:
  static constexpr int WIDTH  = 800;
  static constexpr int HEIGHT = 600;

  FirstApp();
  ~FirstApp();
  FirstApp(const FirstApp&)            = delete;
  FirstApp& operator=(const FirstApp&) = delete;

  void run();

private:
  LgeWindow lgeWindow{"First App", WIDTH, HEIGHT};
  LgeDevice lgeDevice{lgeWindow};
  LgeRenderer lgeRenderer{lgeWindow, lgeDevice};

  void loadGameObjects();

  std::unique_ptr<LgeDescriptorPool> globalPool{};
  LgeGameObject::Map gameObjects;

  std::string relativeModelPath = "../../assets/models/";
};
} // namespace lge