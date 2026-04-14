#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 model;         
} pc;



layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in vec3 instancePos;   // from InstanceData
layout(location = 4) in vec3 instanceRot;   // from InstanceData
layout(location = 5) in vec3 instanceScale; // from InstanceData

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

mat4 translationMatrix(vec3 t) {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                t.x, t.y, t.z, 1.0);
}

mat4 rotationY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(c, 0.0, s, 0.0,
                0.0, 1.0, 0.0, 0.0,
                -s, 0.0, c, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 scaleMatrix(vec3 s) {
    return mat4(s.x, 0.0, 0.0, 0.0,
                0.0, s.y, 0.0, 0.0,
                0.0, 0.0, s.z, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

void main() {

    mat4 m = translationMatrix(instancePos) *
             rotationY(instanceRot.y) *
             scaleMatrix(instanceScale);

    m = pc.model * m;

    gl_Position = ubo.proj * ubo.view * m * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}