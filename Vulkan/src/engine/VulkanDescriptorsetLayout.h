
#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <unordered_map>

class VulkanDescriptorSetLayout {
public:
    struct Builder {
    public:
        Builder(VkDevice device) : device{ device } {}

        Builder& addBinding(
            uint32_t binding,
            VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags,
            uint32_t count = 1);
        std::unique_ptr<VulkanDescriptorSetLayout> build() const;

    private:
        VkDevice device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    VulkanDescriptorSetLayout(
        VkDevice device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~VulkanDescriptorSetLayout();

    VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
    VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    VkDevice device;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class VulkanDescriptorWriter;
};