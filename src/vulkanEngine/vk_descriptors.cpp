#include "vk_descriptors.hpp"

// std
#include <cassert>
#include <stdexcept>

namespace xev {

// *************** Descriptor Set Layout Builder *********************

XevDescriptorSetLayout::Builder& XevDescriptorSetLayout::Builder::addBinding(
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

std::unique_ptr<XevDescriptorSetLayout> XevDescriptorSetLayout::Builder::build() const {
  return std::make_unique<XevDescriptorSetLayout>(xevDevice, bindings);
}

// *************** Descriptor Set Layout *********************

XevDescriptorSetLayout::XevDescriptorSetLayout(
    XevDevice& xevDevice,
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : xevDevice{xevDevice}, bindings{bindings} {
  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
  for (auto kv : bindings) { setLayoutBindings.push_back(kv.second); }

  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
  descriptorSetLayoutInfo.pBindings    = setLayoutBindings.data();

  if (vkCreateDescriptorSetLayout(
          xevDevice.device(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

XevDescriptorSetLayout::~XevDescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(xevDevice.device(), descriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

XevDescriptorPool::Builder&
XevDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
  return *this;
}

XevDescriptorPool::Builder&
XevDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags) {
  poolFlags = flags;
  return *this;
}

XevDescriptorPool::Builder& XevDescriptorPool::Builder::setMaxSets(uint32_t count) {
  maxSets = count;
  return *this;
}

std::unique_ptr<XevDescriptorPool> XevDescriptorPool::Builder::build() const {
  return std::make_unique<XevDescriptorPool>(xevDevice, maxSets, poolFlags, poolSizes);
}

// *************** Descriptor Pool *********************

XevDescriptorPool::XevDescriptorPool(
    XevDevice& xevDevice, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
    const std::vector<VkDescriptorPoolSize>& poolSizes)
    : xevDevice{xevDevice} {
  VkDescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  descriptorPoolInfo.pPoolSizes    = poolSizes.data();
  descriptorPoolInfo.maxSets       = maxSets;
  descriptorPoolInfo.flags         = poolFlags;

  if (vkCreateDescriptorPool(
          xevDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

XevDescriptorPool::~XevDescriptorPool() {
  vkDestroyDescriptorPool(xevDevice.device(), descriptorPool, nullptr);
}

bool XevDescriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = descriptorPool;
  allocInfo.pSetLayouts        = &descriptorSetLayout;
  allocInfo.descriptorSetCount = 1;

  // Might want to create a "DescriptorPoolManager" class that handles this case, and
  // builds a new pool whenever an old pool fills up. But this is beyond our current scope
  if (vkAllocateDescriptorSets(xevDevice.device(), &allocInfo, &descriptor) !=
      VK_SUCCESS) {
    return false;
  }
  return true;
}

void XevDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
  vkFreeDescriptorSets(
      xevDevice.device(), descriptorPool, static_cast<uint32_t>(descriptors.size()),
      descriptors.data());
}

void XevDescriptorPool::resetPool() {
  vkResetDescriptorPool(xevDevice.device(), descriptorPool, 0);
}

// *************** Descriptor Writer *********************

XevDescriptorWriter::XevDescriptorWriter(
    XevDescriptorSetLayout& setLayout, XevDescriptorPool& pool)
    : setLayout{setLayout}, pool{pool} {}

XevDescriptorWriter&
XevDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
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

XevDescriptorWriter&
XevDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo) {
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

bool XevDescriptorWriter::build(VkDescriptorSet& set) {
  bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
  if (!success) { return false; }
  overwrite(set);
  return true;
}

void XevDescriptorWriter::overwrite(VkDescriptorSet& set) {
  for (auto& write : writes) { write.dstSet = set; }
  vkUpdateDescriptorSets(
      pool.xevDevice.device(), writes.size(), writes.data(), 0, nullptr);
}

} // namespace xev
