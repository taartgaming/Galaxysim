#version 450

layout(push_constant) uniform PushConstants {
    mat4 model;         
} pc;


layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * pc.model * vec4(inPosition);
    gl_PointSize = 5.0;
    fragColor = inColor;
} 