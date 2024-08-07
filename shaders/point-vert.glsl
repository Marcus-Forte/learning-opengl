#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vColorOut;

uniform mat4 transform;
uniform float point_size;

void main() {
  gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  gl_PointSize = point_size;
  vColorOut = vec3(aColor.x, aColor.y, aColor.z);
}