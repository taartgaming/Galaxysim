#include "VulkanComputePipeline.h"
#include <fstream>
#include <stdexcept>

VulkanComputePipeline::VulkanComputePipeline(
    VkDevice device,
    const std::string& computeFilepath,
    VkPipelineLayout layout) : device{ device } {

    auto computeShaderCode = readFile(computeFilepath);
    VkShaderModule computeShaderModule = createShaderModule(computeShaderCode);

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = layout;
    pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    pipelineInfo.stage.module = computeShaderModule;
    pipelineInfo.stage.pName = "main";

    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute pipeline!");
    }

    vkDestroyShaderModule(device, computeShaderModule, nullptr);
}

VulkanComputePipeline::~VulkanComputePipeline() {
    //vkDestroyPipeline(device, computePipeline, nullptr);
}

void VulkanComputePipeline::bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
}

void VulkanComputePipeline::dispatch(VkCommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z) {
    vkCmdDispatch(commandBuffer, x, y, z);
}

// 1. Add the prefix to createShaderModule
VkShaderModule VulkanComputePipeline::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

// 2. Add the prefix to readFile
std::vector<char> VulkanComputePipeline::readFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filepath);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}