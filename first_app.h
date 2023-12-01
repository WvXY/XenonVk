#pragma once

#include "lge_window.h"
#include "lge_pipeline.h"
#include "lge_device.h"

#include <string>
#include <filesystem>

namespace lge{
    class FirstApp{
      public:
        static constexpr int WIDTH = 1280;
        static constexpr int HEIGHT = 720;

        void run();

      private:
        const std::string rootPath = std::filesystem::current_path().parent_path().string();
        const std::string vertShaderSrc = "\\shaders\\simple_shader.vert.spv";
        const std::string fragShaderSrc = "\\shaders\\simple_shader.frag.spv";

        LgeWindow lgeWindow{"First App", WIDTH, HEIGHT};
        LgeDevice lgeDevice{lgeWindow};

        LgePipeline lgePipeline{
          lgeDevice,
          rootPath + vertShaderSrc,
          rootPath + fragShaderSrc,
          LgePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
        };

    };
}