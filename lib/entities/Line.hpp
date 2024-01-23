#pragma once

#include "entity.hpp"
#include "layouts/lineAttribute.hpp"
#include "shaderLoader.hpp"

namespace entity {
class Line : public Entity {
 public:
  Line(float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z)
      : p0{p0_x, p0_y, p0_z}, p1{p1_x, p1_y, p1_z} {
    init();
  }

  // virtual ~Line() {
  //   delete shader_program_;
  // }

 private:
  void init() {
    GLLineData line_data{p0[0], p0[0], p0[2], p1[0], p1[1], p1[2]};
    VertexBufferLayout layout_;
    layout_.addAttribute(VertexBufferLayout::float_, 3);
    vertex_object_.setLayout(layout_);
    vertex_object_.setData(&line_data, 2, sizeof(float) * 3);

    shaderLoader line_vertex_shader("../shaders/line_vertex.glsl", shaderLoader::ShaderType::VERTEX);
    auto lineVertexShader = line_vertex_shader.compile();

    shaderLoader fragment_shader("../shaders/fragment.glsl", shaderLoader::ShaderType::FRAGMENT);
    auto fragmentShader = fragment_shader.compile();

    // shader_program_ = new ShaderProgram(lineVertexShader, fragmentShader);

    // program_ = std::make_unique<ShaderProgram>(lineVertexShader, fragmentShader);

    // setShaderProgram(program_.get());
    gl_draw_type_ = GL_LINES;
  }
  const float p0[3];
  const float p1[3];
};
}  // namespace entity