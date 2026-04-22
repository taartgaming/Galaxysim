#pragma once
#include "VulkanBuffer.h"
#include "VulkanDescriptorWriter.h"

#include "VulkanComputePipeline.h"
#include "../VulkanConfig.h"
#include "VulkanUtils.hpp"
#include "../Particle.hpp"
#include <memory>
#include <vector>

class VulkanParticleSystem {
public:
    VulkanParticleSystem(
        VkDevice device,
        VkPhysicalDevice physDevice,
        VulkanDescriptorPool& pool,
        VulkanDescriptorSetLayout& computeLayout,
        VkCommandPool commandPool,      // <--- Add this
        VkQueue graphicsQueue,          // <--- Add this
        uint32_t count);

    ~VulkanParticleSystem();

    void setModelMatrix(const glm::mat4& model) { config.modelMatrix = model; }

    const glm::mat4& getModelMatrix() const { return config.modelMatrix; }

    void updateLogic(glm::vec3 cameraPos);

    // The logic update (Compute)
    void update(VkCommandBuffer cmd, VkPipelineLayout computeLayout, float dt, uint32_t currentFrame, uint32_t activeCount);

    // The visual render (Graphics)
    void draw(VkCommandBuffer cmd, VkPipelineLayout graphicsLayout, uint32_t currentFrame, uint32_t activeCount);

private:
    struct Configuration {
        glm::mat4 modelMatrix{ 1.f };
    } config;

    VkDevice device;
    uint32_t particleCount;

    bool isHighDetail = false;

    // Each galaxy has its own buffers
    std::vector<std::unique_ptr<VulkanBuffer>> storageBuffers;

    // Each galaxy has its own descriptor sets to point to its specific buffers
    std::vector<VkDescriptorSet> computeDescriptorSets;
};