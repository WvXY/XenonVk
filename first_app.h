#pragma once

#include "lge_window.h"

namespace lge{
    class FirstApp{
      public:
        static constexpr int WIDTH = 1280;
        static constexpr int HEIGHT = 720;

        void run();

      private:
        LgeWindow lgeWindow{"First App", WIDTH, HEIGHT};
    };
}