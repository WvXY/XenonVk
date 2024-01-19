#include "lge_descriptors.hpp"

// std
#include <cassert>
#include <stdexcept>

namespace lge {

// *************** Descriptor Set Layout Builder *********************

LgeDescriptorSetLayout::Builder& LgeDescriptorSetLayout::Builder::addBinding(
    uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags,
    uint32_t count) {
  assert(bindings.count(binding) == 0 && "Binding already in use");
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding         = binding;
  layoutBinding.descriptorType  = descriptorType;
  layoutBinding.descriptorCount = count;
  layoutBinding.stageFlags      = stageFlags;
  bindings[binding]             = layoutBinding;
  return *this;
}

std::unique_ptr<LgeDescriptorSetLayout> LgeDescriptorSetLayout::Builder::build() const {
  return std::make_unique<LgeDescriptorSetLayout>(lgeDevice, bindings);
}

// *************** Descriptor Set Layout *********************

LgeDescriptorSetLayout::LgeDescriptorSetLayout(
    LgeDevice& lgeDevice,
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : lgeDevice{lgeDevice}, bindings{bindings} {
  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
  for (auto kv : bindings) { setLayoutBindings.push_back(kv.second); }

  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
  descriptorSetLayoutInfo.pBindings    = setLayoutBindings.data();

  if (vkCreateDescriptorSetLayout(
          lgeDevice.device(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

LgeDescriptorSetLayout::~LgeDescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(lgeDevice.device(), descriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

LgeDescriptorPool::Builder&
LgeDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
  return *this;
}

LgeDescriptorPool::Builder&
LgeDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags) {
  poolFlags = flags;
  return *this;
}
LgeDescriptorPool::Builder& LgeDescriptorPool::Builder::setMaxSets(uint32_t count) {
  maxSets = count;
  return *this;
}

std::unique_ptr<LgeDescriptorPool> LgeDescriptorPool::Builder::build() const {
  return std::make_unique<LgeDescriptorPool>(lgeDevice, maxSets, poolFlags, poolSizes);
}

// *************** Descriptor Pool *********************

LgeDescriptorPool::LgeDescriptorPool(
    LgeDevice& lgeDevice, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
    const std::vector<VkDescriptorPoolSize>& poolSizes)
    : lgeDevice{lgeDevice} {
  VkDescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  descriptorPoolInfo.pPoolSizes    = poolSizes.data();
  descriptorPoolInfo.maxSets       = maxSets;
  descriptorPoolInfo.flags         = poolFlags;

  if (vkCreateDescriptorPool(
          lgeDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

LgeDescriptorPool::~LgeDescriptorPool() {
  vkDestroyDescriptorPool(lgeDevice.device(), descriptorPool, nullptr);
}

bool LgeDescriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = descriptorPool;
  allocInfo.pSetLayouts        = &descriptorSetLayout;
  allocInfo.descriptorSetCount = 1;

  // Might want to create a "DescriptorPoolManager" class that handles this case, and
  // builds a new pool whenever an old pool fills up. But this is beyond our current scope
  if (vkAllocateDescriptorSets(lgeDevice.device(), &allocInfo, &descriptor) !=
      VK_SUCCESS) {
    return false;
  }
  return true;
}

void LgeDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
  vkFreeDescriptorSets(
      lgeDevice.device(), descriptorPool, static_cast<uint32_t>(descriptors.size()),
      descriptors.data());
}

void LgeDescriptorPool::resetPool() {
  vkResetDescriptorPool(lgeDevice.device(), descriptorPool, 0);
}

// *************** Descriptor Writer *********************

LgeDescriptorWriter::LgeDescriptorWriter(
    LgeDescriptorSetLayout& setLayout, LgeDescriptorPool& pool)
    : setLayout{setLayout}, pool{pool} {}

LgeDescriptorWriter&
LgeDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
  assert(
      setLayout.bindings.count(binding) == 1 &&
      "Layout does not contain specified binding");

  auto& bindingDescription = setLayout.bindings[binding];

  assert(
      bindingDescription.descriptorCount == 1 &&
      "Binding single descriptor info, but binding expects multiple");

  VkWriteDescriptorSet write{};
  write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType  = bindingDescription.descriptorType;
  write.dstBinding      = binding;
  write.pBufferInfo     = bufferInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

LgeDescriptorWriter&
LgeDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo) {
  assert(
      setLayout.bindings.count(binding) == 1 &&
      "Layout does not contain specified binding");

  auto& bindingDescription = setLayout.bindings[binding];

  assert(
      bindingDescription.descriptorCount == 1 &&
      "Binding single descriptor info, but binding expects multiple");

  VkWriteDescriptorSet write{};
  write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType  = bindingDescription.descriptorType;
  write.dstBinding      = binding;
  write.pImageInfo      = imageInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

bool LgeDescriptorWriter::build(VkDescriptorSet& set) {
  bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
  if (!success) { return false; }
  overwrite(set);
  return true;
}

void LgeDescriptorWriter::overwrite(VkDescriptorSet& set) {
  for (auto& write : writes) { write.dstSet = set; }
  vkUpdateDescriptorSets(
      pool.lgeDevice.device(), writes.size(), writes.data(), 0, nullptr);
}

} // namespace lge