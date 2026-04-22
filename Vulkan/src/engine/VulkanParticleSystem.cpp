#include "VulkanParticleSystem.h"
#include <random>
#include <ctime>

VulkanParticleSystem::VulkanParticleSystem(
    VkDevice device,
    VkPhysicalDevice physDevice,
    VulkanDescriptorPool& pool,
    VulkanDescriptorSetLayout& computeLayout,
    VkCommandPool commandPool,
    VkQueue graphicsQueue,
    uint32_t count)
    : device{ device }, particleCount{ count } {

    VkDeviceSize bufferSize = sizeof(Particle) * particleCount;

    // --- 1. GENERATE INITIAL DATA ---
    std::vector<Particle> initialParticles(particleCount);
    std::default_random_engine rndEngine(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<float> rndDist(0.0f, 1.0f);

    for (auto& p : initialParticles) {
        float r = 10.f * sqrt(rndDist(rndEngine));
        float theta = 6.2831f * rndDist(rndEngine);

        float orbitSpeed = 0.3f / (r + 0.1f);

                // 2. Stochastic Clutter (The "Spread")
        // These values are static—they don't change, they just offset the math
        float spreadX = (rndDist(rndEngine) - 0.5f) * 0.12f;
        float spreadY = (rndDist(rndEngine) - 0.5f) * 0.12f;
        float spreadZ = (rndDist(rndEngine) - 0.5f) * 0.4f;

        p.position = glm::vec4(0.0f, 0.0f, spreadZ, 1.0f);

        glm::vec3 baseColor = glm::mix(glm::vec3(1.0f, 0.8f, 0.5f), glm::vec3(0.5f, 0.7f, 1.0f), r);
        p.color = glm::vec4(baseColor, spreadY);

        p.velocity = glm::vec4(r, theta, orbitSpeed, spreadX);
    }



    // --- 2. STAGING (CPU to GPU Bridge) ---
    // Matches your (device, physDevice, size, usage, properties)
    VulkanBuffer stagingBuffer{
        device,
        physDevice,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    stagingBuffer.map();
    stagingBuffer.copyTo(initialParticles.data(), bufferSize);
    stagingBuffer.unmap();

    // --- 3. CREATE PERSISTENT GPU BUFFERS ---
    storageBuffers.resize(EngineConfig::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < EngineConfig::MAX_FRAMES_IN_FLIGHT; i++) {
        storageBuffers[i] = std::make_unique<VulkanBuffer>(
            device,
            physDevice,
            bufferSize,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        VulkanUtils::copyBuffer(device, commandPool, graphicsQueue, stagingBuffer.getBuffer(), storageBuffers[i]->getBuffer(), bufferSize);
    }

    // --- 4. BUILD DESCRIPTORS ---
    computeDescriptorSets.resize(EngineConfig::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < EngineConfig::MAX_FRAMES_IN_FLIGHT; i++) {
        auto prevInfo = storageBuffers[(i + 1) % 2]->descriptorInfo();
        auto currInfo = storageBuffers[i]->descriptorInfo();

        VulkanDescriptorWriter(computeLayout, pool)
            .writeBuffer(0, &prevInfo)
            .writeBuffer(1, &currInfo)
            .build(computeDescriptorSets[i]);
    }

    stagingBuffer.cleanup();
}

VulkanParticleSystem::~VulkanParticleSystem() {
    for (auto& buffer : storageBuffers) {
        if (buffer) {
            buffer->cleanup();
        }
    }
}

void VulkanParticleSystem::updateLogic(glm::vec3 cameraPos) {
    float distance = glm::distance(cameraPos, glm::vec3(config.modelMatrix[3]));

    if (distance < 1.0f) {
        // High Detail Mode: Maybe spawn "Solar System" particles
        isHighDetail = true;
    }
    else {
        isHighDetail = false;
    }
}

void VulkanParticleSystem::update(VkCommandBuffer cmd, VkPipelineLayout computeLayout, float dt, uint32_t currentFrame, uint32_t activeCount) {
    // Push the DeltaTime (Push Constant)
    vkCmdPushConstants(cmd, computeLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(float), &dt);

    // Bind this galaxy's descriptor set
    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        computeLayout,
        0, 1,
        &computeDescriptorSets[currentFrame],
        0, nullptr
    );

    // Dispatch the compute shader
    vkCmdDispatch(cmd, (activeCount + 255) / 256, 1, 1);
}

void VulkanParticleSystem::draw(VkCommandBuffer cmd, VkPipelineLayout graphicsLayout, uint32_t currentFrame, uint32_t activeCount) {
    // 1. Push Model Matrix (Push Constant)
    vkCmdPushConstants(
        cmd,
        graphicsLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(glm::mat4),
        &config.modelMatrix
    );

    // 2. Bind current storage buffer as a vertex buffer
    VkBuffer buffers[] = { storageBuffers[currentFrame]->getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, buffers, offsets);

    // 3. Draw
    vkCmdDraw(cmd, activeCount, 1, 0, 0);
}