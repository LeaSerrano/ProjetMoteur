#version 330 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 o_uv0;

out vec3 FragPos;
out vec3 FragNormal;
out vec3 FragLightDir;

vec3 lightDirection = vec3(-0.2, 0.8, -0.3);

void main(){

    gl_Position = projection * view * model * vec4(position, 1.0);
    o_uv0 = uv0;
    FragNormal = mat3(transpose(inverse(model))) * normal;
    FragLightDir = mat3(transpose(inverse(view))) * lightDirection;

}
