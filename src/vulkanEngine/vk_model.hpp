#pragma once

#include "vk_buffer.hpp"
#include "vk_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace xev {
class XevModel {
public:
  struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 uv{};

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

    bool operator==(const Vertex& other) const {
      return position == other.position && color == other.color &&
             normal == other.normal && uv == other.uv;
    }
  };

  struct Builder {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

    void loadModel(const std::string& filepath);
  };

  XevModel(XevDevice& device, const Builder& builder);
  ~XevModel();
  XevModel(const XevModel&)            = delete;
  XevModel& operator=(const XevModel&) = delete;

  static std::unique_ptr<XevModel>
  createModelFromFile(XevDevice& device, const std::string& filepath);

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffers(const std::vector<Vertex>& vertices);
  void createIndexBuffers(const std::vector<uint32_t>& indices);

  XevDevice& xevDevice;

  std::unique_ptr<XevBuffer> vertexBuffer;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  std::unique_ptr<XevBuffer> indexBuffer;
  uint32_t indexCount;
};
} // namespace xev
