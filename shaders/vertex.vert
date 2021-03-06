#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

uniform mat4 modelViewProjMat;

out vec3 vColor;

void main()
{
    gl_Position = modelViewProjMat * vec4(inPos, 1.0);
    vColor = inColor;
}