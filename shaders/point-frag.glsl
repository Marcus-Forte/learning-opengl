#version 330 core

out vec4 colorOut;
in vec3 vColorOut;

// gl_PointCoord holds fragment coodinates relative to the center of a point primitive.
void main() {
  if (dot(gl_PointCoord - 0.5, gl_PointCoord - 0.5) > 0.25)
    discard;
  else
    colorOut = vec4(vColorOut, 1.0);
}