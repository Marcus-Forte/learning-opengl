#include "shaderProgram.hh"

#include "shaderLoader.hh"

ShaderProgram::ShaderProgram(const std::filesystem::path& vertex_shader_path,
                             const std::filesystem::path& fragment_shader_path) {
  // use basic shader
  shaderLoader vertex_shader(vertex_shader_path, shaderLoader::ShaderType::VERTEX);
  auto vertexShaderId = vertex_shader.compile();
  shaderLoader fragment_shader(fragment_shader_path, shaderLoader::ShaderType::FRAGMENT);
  auto fragmentShaderId = fragment_shader.compile();

  shader_program_id_ = glCreateProgram();

  glAttachShader(shader_program_id_, vertexShaderId);
  glAttachShader(shader_program_id_, fragmentShaderId);
  glLinkProgram(shader_program_id_);

  int success;
  char infoLog[512];
  glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_id_, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::PROGRAM_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
}