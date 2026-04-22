#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Expose getters for your Frustum Culling and Projection
    glm::vec3 getPosition() const { return pos; }
    glm::vec3 getFront() const { return front; }
    float getFov() const { return fov; }
    glm::mat4 getViewMatrix() const { return glm::lookAt(pos, pos + front, up); }

    // Input processing
    void processKeyboard(GLFWwindow* window, float deltaTime) {
        float velocity = speed * deltaTime;

        // Standard WASD
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos += front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos -= front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos -= right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos += right * velocity;

        // Vertical Movement (Space = Up, Left Shift = Down)
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) pos += worldUp * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) pos -= worldUp * velocity;
    }

    void processMouse(float xoffset, float yoffset) {
        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;

        // Constrain pitch so the screen doesn't flip
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateCameraVectors();
    }

    void processScroll(float yoffset) {
        // yoffset is usually +1 or -1 from the mouse wheel
        fov -= yoffset * 2.0f;

        // Clamp FOV so you can't zoom in to 0 or invert the screen
        if (fov < 1.0f) fov = 1.0f;
        if (fov > 90.0f) fov = 90.0f;
    }

private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f); // Absolute Up direction

    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 25.0f; // Sped up for space travel!
    float sensitivity = 0.1f;
    float fov = 45.0f;   // Added FOV for zooming

    // Recalculates the vectors based on Euler angles
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        // Recalculate Right and Up vectors
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};