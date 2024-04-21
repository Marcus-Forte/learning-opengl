#pragma once

#include "entity.hh"

namespace entity {
class Axis : public Entity {
 public:
  using Entity::vertex_object_;
  Axis(float ox, float oy, float oz, float length = 1.0f) {
    // Four points needed.
    float axis_data[12]{ox, oy, oz, ox + length, oy, oz, ox, oy + length, oz, ox, oy, oz + length};

    VertexBufferLayout layout_;
    layout_.addAttribute(VertexBufferLayout::float_, 3);  // pos
    vertex_object_.setLayout(layout_);
    vertex_object_.setData(&axis_data, 4, sizeof(float) * 3);

    glGenBuffers(1, &ebo_id_);
    unsigned int sequence[]{
        0, 1, 0, 2, 0, 3  // o->x, o->y, o->z.
    };
    // Store Element index buffer in VAO.
    vertex_object_.bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), sequence, GL_STATIC_DRAW);
    vertex_object_.unbind();

    shaderLoader line_vertex_shader("../shaders/line-vert.glsl", shaderLoader::ShaderType::VERTEX);
    auto lineVertexShader = line_vertex_shader.compile();
    shaderLoader fragment_shader("../shaders/fragment.glsl", shaderLoader::ShaderType::FRAGMENT);
    auto fragmentShader = fragment_shader.compile();

    shader_program_.reset(new ShaderProgram(lineVertexShader, fragmentShader));
  }

  void setAxisWidth(float width) { line_width_ = width; }
  void draw() const override {
    if (!shader_program_) throw std::runtime_error("No shader program set for entity.");
    vertex_object_.bind();
    shader_program_->use();
    glLineWidth(line_width_);
    int uniform_location = glGetUniformLocation(shader_program_->getId(), "color");
    glUniform3f(uniform_location, 1.0f, 0.0f, 0.0f);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
    glUniform3f(uniform_location, 0.0f, 1.0f, 0.0f);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void *)8);  // 2* unsigned int
    glUniform3f(uniform_location, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void *)16);
    vertex_object_.unbind();
  }

 private:
  unsigned int ebo_id_;
  float line_width_ = 5.0f;
};
}  // namespace entity