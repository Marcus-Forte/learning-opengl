#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 vColorOut;

uniform mat4 transform;
uniform vec3 color;

void main()
{
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vColorOut = color;
}