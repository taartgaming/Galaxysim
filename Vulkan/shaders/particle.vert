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
    vec4 worldPos = pc.model * vec4(inPosition.xyz, 1.0);
    vec4 viewPos = ubo.view * worldPos;
    gl_Position = ubo.proj * viewPos;

    // Distance-based point size: Stars get bigger as you get closer
    // 5.0 is base size, divided by the Z-depth in view space
    gl_PointSize = 5.0 * (1.0 / -viewPos.z); 
    
    // Clamp it so they don't disappear when far or become blocks when close
    gl_PointSize = clamp(gl_PointSize, 1.0, 10.0);

    fragColor = inColor;
}