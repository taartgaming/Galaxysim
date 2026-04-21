#pragma once
#include <vulkan/vulkan.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <array>

struct InstanceData {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 1; // Note: Binding 1 is for instances
        bindingDescription.stride = sizeof(InstanceData);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attribs{};

        // Location 3: Position
        attribs[0].binding = 1;
        attribs[0].location = 3;
        attribs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribs[0].offset = offsetof(InstanceData, position);

        // Location 4: Rotation
        attribs[1].binding = 1;
        attribs[1].location = 4;
        attribs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribs[1].offset = offsetof(InstanceData, rotation);

        // Location 5: Scale
        attribs[2].binding = 1;
        attribs[2].location = 5;
        attribs[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribs[2].offset = offsetof(InstanceData, scale);

        return attribs;
    }
};