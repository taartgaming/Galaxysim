#pragma once
#include "VulkanBuffer.h"
#include "VulkanUtils.hpp"
#include "../InstanceData.hpp"
#include <memory>
#include <vector>

class InstanceGroup {
public:
    InstanceGroup(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
        VkQueue graphicsQueue, const std::vector<InstanceData>& instances);

    void destroy();
    VkBuffer getBuffer() const { return instanceBuffer->getBuffer(); }
    uint32_t getCount() const { return count; }

private:
    std::unique_ptr<VulkanBuffer> instanceBuffer;
    uint32_t count;
};