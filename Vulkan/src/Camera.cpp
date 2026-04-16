#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>



#include <chrono>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <array>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <optional>
#include <set>
#include <unordered_map>
#include <random>



class Camera {
public:
    glm::vec3 pos = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 front = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

    float yaw = -135.0f;
    float pitch = -35.0f;
    float speed = 2.5f;
    float sensitivity = 0.1f;

    glm::mat4 getViewMatrix() {
        return glm::lookAt(pos, pos + front, up);
    }

    void processKeyboard(GLFWwindow* window, float deltaTime) {
        float velocity = speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos += front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos -= front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos -= glm::normalize(glm::cross(front, up)) * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos += glm::normalize(glm::cross(front, up)) * velocity;
    }

    void processMouse(float xoffset, float yoffset) {
        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.z = sin(glm::radians(pitch));
        front = glm::normalize(direction);
    }
};