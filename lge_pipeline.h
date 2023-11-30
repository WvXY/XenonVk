#pragma once

#include <string>
#include <vector>

namespace lge {
    class LgePipeline {
        public:
        LgePipeline(
                const std::string& vertex_shader_path,
                const std::string& fragment_shader_path);

        private:
        static std::vector<char> readFile(const std::string& path);

        void createGraphicsPipeline(
                const std::string& vertex_shader_path,
                const std::string& fragment_shader_path);
    };
}