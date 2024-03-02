#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "vertexBufferLayout.h"

class VertexObject {
 public:
  VertexObject() {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);
    glGenBuffers(1, &vbo_id_);
  }

  virtual ~VertexObject() {
    // glDeleteVertexArrays(1, &vao_id_);
    // glDeleteBuffers(1, &vbo_id_);
  }

  void setData(void* data, size_t num_vertices, size_t vertex_size) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, num_vertices * vertex_size, data, GL_STATIC_DRAW);
    num_vertices_ = num_vertices;
    unbind();
  }

  template <class VertexStruct>
  void setLayout(size_t num_attributes, const std::vector<unsigned int>& attribute_num_elements,
                 const std::vector<unsigned int>& attribute_gl_types) {
    if (num_attributes != attribute_num_elements.size() || num_attributes != attribute_gl_types.size())
      throw std::runtime_error("Mismatching attribute sizes.");

    bind();
    unsigned int stride = sizeof(VertexStruct);
    for (unsigned int attributeIndex = 0; attributeIndex < num_attributes; ++attributeIndex) {
      const unsigned int numAttributeElements = attribute_num_elements[attributeIndex];
      const unsigned int attributeType = attribute_gl_types[attributeIndex];
      const unsigned int normalized = GL_FALSE;
      const unsigned int attributeOffset =
          attributeIndex * numAttributeElements * VertexBufferLayout::getsizeFromType(GL_FLOAT);
      glVertexAttribPointer(attributeIndex, numAttributeElements, attributeType, normalized, stride,
                            reinterpret_cast<void*>(attributeOffset));
      glEnableVertexAttribArray(attributeIndex);
      printf("glVertexAttribPointer(%d, %d, %x, %d, %d, %d)\n", attributeIndex, numAttributeElements, attributeType,
             normalized, stride, attributeOffset);
    }
    unbind();
  }

  void setLayout(const VertexBufferLayout& layout) {
    bind();
    for (unsigned int attributeIndex = 0; attributeIndex < layout.getNumAttributes(); ++attributeIndex) {
      const unsigned int attributeNumElements = layout.getAttributeNumElements()[attributeIndex];
      const unsigned int attributeType = layout.getAttributeType()[attributeIndex];
      const unsigned int normalized = GL_FALSE;
      const unsigned int attributeTypeSize = layout.getAttributeTypeSize()[attributeIndex];
      const unsigned int attributeStride = attributeTypeSize * attributeNumElements * layout.getNumAttributes();
      const unsigned int attributeOffset = attributeIndex * attributeTypeSize * attributeNumElements;

      printf("glVertexAttribPointer(%d, %d, %x, %d, %d, %d)\n", attributeIndex, attributeNumElements, attributeType,
             normalized, attributeStride, attributeOffset);
      glVertexAttribPointer(attributeIndex, attributeNumElements, attributeType, normalized, attributeStride,
                            reinterpret_cast<void*>(attributeOffset));

      glEnableVertexAttribArray(attributeIndex);
    }
    unbind();
  }

  void bind() const {
    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
  }

  void unbind() const { glBindBuffer(0, vbo_id_); }

  size_t getNumVertices() const { return num_vertices_; }

 private:
  unsigned int vao_id_;
  unsigned int vbo_id_;
  size_t num_vertices_;
};