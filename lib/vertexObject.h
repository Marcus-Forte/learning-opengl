#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "vertexBufferLayout.h"
/*
Primer on VAOS:
- glBindVertexArray() records subsequent calls from:
- glBindBuffer() followed by glVertexAttribPointer() (they are correlated)
- glEnableVertexAttribArray()
- glBindBuffer(GL_ELEMENT_ARRAY_BUFFER)
- VAO are good expression of scene objects.

Good ref: https://www.khronos.org/opengl/wiki/Vertex_Specification
*/

/// @brief Abstraction of Vertex Array Objects (VAOs).
class VertexObject {
 public:
  VertexObject() {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);
    glGenBuffers(1, &vbo_id_);
  }

  virtual ~VertexObject() {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
  }

  /// @brief set vertex object data.
  /// @param data vertex data to copy from.
  /// @param num_vertices number of vertices.
  /// @param vertex_size size of each vertex in bytes.
  void setData(void* data, size_t num_vertices, size_t vertex_size) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, data, GL_STATIC_DRAW);
    num_vertices_ = num_vertices;
  }

  /// @brief sets the VAO layout.
  /// @tparam VertexStruct struct containing all properties.
  /// @param num_attributes number of vertex attributes.
  /// @param attribute_num_elements vector of number of elements.
  /// @param attribute_gl_types vector of element primitive
  template <class VertexStruct>
  void setLayout(size_t num_attributes, const std::vector<unsigned int>& attribute_num_elements,
                 const std::vector<unsigned int>& attribute_gl_types) {
    if (num_attributes != attribute_num_elements.size() || num_attributes != attribute_gl_types.size())
      throw std::runtime_error("Mismatching attribute sizes.");

    bind();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    unsigned int stride = sizeof(VertexStruct);
    unsigned int attributeOffset = 0;
    for (unsigned int attributeIndex = 0; attributeIndex < num_attributes; ++attributeIndex) {
      const unsigned int numAttributeElements = attribute_num_elements[attributeIndex];
      const unsigned int attributeType = attribute_gl_types[attributeIndex];
      const unsigned int normalized = GL_FALSE;

      glVertexAttribPointer(attributeIndex, numAttributeElements, attributeType, normalized, stride,
                            reinterpret_cast<void*>(attributeOffset));
      glEnableVertexAttribArray(attributeIndex);
      // printf("glVertexAttribPointer(%d, %d, %x, %d, %d, %d)\n", attributeIndex, numAttributeElements, attributeType,
      //        normalized, stride, attributeOffset);
      attributeOffset += numAttributeElements * VertexBufferLayout::getsizeFromType(attributeType);
    }
    unbind();
  }

  /// @brief Sets the VAO layout.
  /// @param layout
  void setLayout(const VertexBufferLayout& layout) {
    bind();
    unsigned int attributeOffset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    for (unsigned int attributeIndex = 0; attributeIndex < layout.getNumAttributes(); ++attributeIndex) {
      const unsigned int numAttributeElements = layout.getAttributeNumElements()[attributeIndex];
      const unsigned int attributeType = layout.getAttributeType()[attributeIndex];
      const unsigned int normalized = GL_FALSE;
      const unsigned int attributeTypeSize = layout.getAttributeTypeSize()[attributeIndex];
      const unsigned int attributeStride = attributeTypeSize * numAttributeElements * layout.getNumAttributes();

      glVertexAttribPointer(attributeIndex, numAttributeElements, attributeType, normalized, attributeStride,
                            reinterpret_cast<void*>(attributeOffset));
      glEnableVertexAttribArray(attributeIndex);
      // printf("glVertexAttribPointer(%d, %d, %x, %d, %d, %d)\n", attributeIndex, attributeNumElements, attributeType,
      //        normalized, attributeStride, attributeOffset);
      attributeOffset += numAttributeElements * attributeTypeSize;
    }
    unbind();
  }

  /// @brief bind VAO object.
  void bind() const { glBindVertexArray(vao_id_); }
  /// @brief unbind VAO object.
  void unbind() const { glBindVertexArray(0); }

  size_t getNumVertices() const { return num_vertices_; }

 private:
  unsigned int vao_id_;
  unsigned int vbo_id_;
  size_t num_vertices_;
};