#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    // gl_PointCoord goes from (0,0) to (1,1) across the point sprite
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);
    
    // Discard pixels outside the circle
    if (dist > 0.5) discard;
    
    // Soft falloff toward edges
    float alpha = 1.0 - smoothstep(0.3, 0.5, dist);
    outColor = vec4(fragColor.rgb, fragColor.a * alpha);
} 