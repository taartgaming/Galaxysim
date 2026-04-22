#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

class VulkanComputePipeline {
public:
    VulkanComputePipeline(
        VkDevice device,
        const std::string& computeFilepath,
        VkPipelineLayout layout);

    ~VulkanComputePipeline();

    VulkanComputePipeline(const VulkanComputePipeline&) = delete;
    VulkanComputePipeline& operator=(const VulkanComputePipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void dispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

private:
    VkDevice device;
    VkPipeline computePipeline;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filepath);
};