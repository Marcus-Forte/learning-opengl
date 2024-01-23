#version 330 core

out vec4 colorOut;
in vec3 vColorOut;

void main()
{
    colorOut = vec4(vColorOut.x, vColorOut.y, vColorOut.z, 1.0f);
} 