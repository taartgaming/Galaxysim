#include "VulkanDescriptorSetLayout.h"
#include <stdexcept>

// --- Builder Methods ---
VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::addBinding(
    uint32_t binding,
    VkDescriptorType descriptorType,
    VkShaderStageFlags stageFlags,
    uint32_t count) {

    // Check if binding already exists
    if (bindings.count(binding) > 0) {
        throw std::runtime_error("Binding already in use");
    }

    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;
    return *this;
}

std::unique_ptr<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::build() const {
    return std::make_unique<VulkanDescriptorSetLayout>(device, bindings);
}

// --- Layout Methods ---
VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(
    VkDevice device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : device{ device }, bindings{ bindings } {

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
    for (auto kv : bindings) {
        setLayoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(device, &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}
