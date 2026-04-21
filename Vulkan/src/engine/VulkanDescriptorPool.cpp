#include "VulkanDescriptorPool.h"
#include <stdexcept>

// --- Builder Methods ---
VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::addPoolSize(
    VkDescriptorType descriptorType, uint32_t count) {
    poolSizes.push_back({ descriptorType, count });
    return *this;
}

VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::setMaxSets(uint32_t count) {
    maxSets = count;
    return *this;
}

std::unique_ptr<VulkanDescriptorPool> VulkanDescriptorPool::Builder::build() const {
    return std::make_unique<VulkanDescriptorPool>(device, maxSets, poolFlags, poolSizes);
}

VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::setPoolFlags(
    VkDescriptorPoolCreateFlags flags) {
    poolFlags = flags;
    return *this;
}

// --- Pool Methods ---
VulkanDescriptorPool::VulkanDescriptorPool(
    VkDevice device,
    uint32_t maxSets,
    VkDescriptorPoolCreateFlags poolFlags,
    const std::vector<VkDescriptorPoolSize>& poolSizes)
    : device{ device } {

    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    if (vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

VulkanDescriptorPool::~VulkanDescriptorPool() {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

bool VulkanDescriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might fail if pool is empty
    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptor) != VK_SUCCESS) {
        return false;
    }
    return true;
}

void VulkanDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
    // This ONLY works if the pool was created with the VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT flag
    vkFreeDescriptorSets(
        device,
        descriptorPool,
        static_cast<uint32_t>(descriptors.size()),
        descriptors.data());
}

void VulkanDescriptorPool::resetPool() {
    // This clears every descriptor set allocated from this pool at once.
    // Very efficient for frame-based or scene-based cleanup.
    vkResetDescriptorPool(device, descriptorPool, 0);
}
