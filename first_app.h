#pragma once

#include "lge_window.h"
#include "lge_pipeline.h"

#include <string>
#include <filesystem>

namespace lge{
    class FirstApp{
      public:
        static constexpr int WIDTH = 1280;
        static constexpr int HEIGHT = 720;

        void run();

      private:
        // TODO: make root path configurable
        std::string rootPath = std::filesystem::current_path().parent_path().string();
        std::string vertShaderSrc = "shaders\\simple_shader.vert.spv";
        std::string fragShaderSrc = "shaders\\simple_shader.frag.spv";

        LgeWindow lgeWindow{"First App", WIDTH, HEIGHT};
        LgePipeline lgePipeline{
          rootPath + "\\" + vertShaderSrc, rootPath + "\\" + fragShaderSrc};
    };
}