#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fPos;
out vec3 fNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormal;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    fPos = vec3(uModel * vec4(aPos, 1.0));
    fNormal = uNormal * aNormal;
}