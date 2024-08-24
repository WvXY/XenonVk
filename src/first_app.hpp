#pragma once

#include <memory>
#include <string>
#include <vector>

#include "primitive/vk_game_object.hpp"
#include "vk_descriptors.hpp"
#include "vk_device.hpp"
#include "vk_renderer.hpp"
#include "vk_window.hpp"

namespace xev {
  class FirstApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();
    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

  private:
    XevWindow xevWindow{ "VkDemo3", WIDTH, HEIGHT };
    XevDevice xevDevice{ xevWindow };
    XevRenderer xevRenderer{ xevWindow, xevDevice };

    void loadGameObjects();

    std::unique_ptr<XevDescriptorPool> globalPool{};
    XevGameObject::Map gameObjects;

    const std::string relativeModelPath = "../../assets/models/";
  };
} // namespace xev
