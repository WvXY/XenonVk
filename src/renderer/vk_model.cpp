#include "vk_model.hpp"
#include "vk_utils.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
template <> struct hash<xev::XevModel::Vertex> {
  size_t operator()(xev::XevModel::Vertex const& vertex) const {
    size_t seed = 0;
    xev::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
    return seed;
  }
};
} // namespace std

namespace xev {
XevModel::XevModel(XevDevice& device, const XevModel::Builder& builder)
    : xevDevice{device} {
  createVertexBuffers(builder.vertices);
  createIndexBuffers(builder.indices);
}

XevModel::~XevModel() {}

std::unique_ptr<XevModel>
XevModel::createModelFromFile(XevDevice& device, const std::string& filepath) {
  Builder builder{};
  builder.loadModel(filepath);
  std::cout << "Model size= " << builder.vertices.size() << " vertices\n";
  return std::make_unique<XevModel>(device, builder);
}

void XevModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[]     = {vertexBuffer->getBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

  if (hasIndexBuffer) {
    vkCmdBindIndexBuffer(
        commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
  }
}

void XevModel::draw(VkCommandBuffer commandBuffer) {
  if (hasIndexBuffer) {
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    return;
  } else {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
  }
}

void XevModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be at least 3");

  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  uint32_t vertexSize     = sizeof(vertices[0]);

  XevBuffer stagingBuffer{
      xevDevice, vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void*)vertices.data());

  vertexBuffer = std::make_unique<XevBuffer>(
      xevDevice, vertexSize, vertexCount,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  xevDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
}

void XevModel::createIndexBuffers(const std::vector<uint32_t>& indices) {
  indexCount     = static_cast<uint32_t>(indices.size());
  hasIndexBuffer = indexCount > 0;
  if (!hasIndexBuffer) return;

  VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
  uint32_t indexSize      = sizeof(indices[0]);

  XevBuffer stagingBuffer{
      xevDevice, indexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void*)indices.data());

  indexBuffer = std::make_unique<XevBuffer>(
      xevDevice, indexSize, indexCount,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  xevDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
}

void XevModel::createTextureImage(const std::string& filepath) {
  int texWidth, texHeight, texChannels;
  stbi_uc* pixels =
      stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  if (!pixels) { throw std::runtime_error("failed to load texture image!"); }

  uint32_t imageSize      = texWidth * texHeight * 4;
  size_t pixelSize        = sizeof(pixels[0]);
  VkDeviceSize bufferSize = sizeof(pixels[0]) * imageSize;

  XevBuffer stagingBuffer{
      xevDevice, pixelSize, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

  stagingBuffer.map();
  stagingBuffer.writeToBuffer((void*)pixels);

  textureBuffer = std::make_unique<XevBuffer>(
      xevDevice, pixelSize, imageSize,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  stbi_image_free(pixels);

  xevDevice.copyBuffer(stagingBuffer.getBuffer(), textureBuffer->getBuffer(), bufferSize);
}

std::vector<VkVertexInputBindingDescription> XevModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding   = 0;
  bindingDescriptions[0].stride    = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
XevModel::Vertex::getAttributeDescriptions() {
  return {// location, binding, format, offset
          {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
          {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)},
          {2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)},
          {3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)}};
}

void XevModel::Builder::loadModel(const std::string& filepath) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
    throw std::runtime_error(warn + err);
  }

  vertices.clear();
  indices.clear();

  std::unordered_map<Vertex, uint32_t> uniqueVertices{};

  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      Vertex vertex{};

      if (index.vertex_index >= 0) {
        vertex.position = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2]};

        vertex.color = {
            attrib.colors[3 * index.vertex_index + 0],
            attrib.colors[3 * index.vertex_index + 1],
            attrib.colors[3 * index.vertex_index + 2]};
      }
      if (index.normal_index >= 0) {
        vertex.normal = {
            attrib.normals[3 * index.normal_index + 0],
            attrib.normals[3 * index.normal_index + 1],
            attrib.normals[3 * index.normal_index + 2]};
      }
      if (index.texcoord_index >= 0) {
        vertex.uv = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
      }

      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }
      indices.push_back(uniqueVertices[vertex]);
    }
  }
}

} // namespace xev
