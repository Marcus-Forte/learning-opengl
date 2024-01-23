#pragma once

#include <GL/gl.h>

#include "vertexBufferLayout.h"

class VertexBuffer {
 public:
  VertexBuffer() { glGenBuffers(1, &vbo_id_); }

  void setData(void* data, size_t bytes) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_STATIC_DRAW);
  }

  void setLayout(const VertexBufferLayout& layout) {
    bind();
    for (int i = 0; i < layout.getNumAttributes(); ++i) {
      const unsigned int attributeIndex = i;
      const unsigned int numAttributeElements =
          layout.getAttributeNumElements()[i];
      const unsigned int attributeType = layout.getAttributeType()[i];
      const unsigned int normalized = GL_FALSE;
      const unsigned int attributeTypeSize = layout.getAttributeTypeSize()[i];
      const unsigned int attributeOffset = attributeIndex * attributeTypeSize *
                                           numAttributeElements *
                                           numAttributeElements;

      printf("glVertexAttribPointer(%d, %d, %x, %d, %d, %d)\n", attributeIndex,
             numAttributeElements, attributeType, normalized, 0,
             attributeOffset);
      glVertexAttribPointer(attributeIndex, numAttributeElements, attributeType,
                            normalized, 0, (void*)attributeOffset);

      glEnableVertexAttribArray(i);
    }
    unbind();
  }

  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, vbo_id_); }

  void unbind() const { glBindBuffer(0, vbo_id_); }

 private:
  unsigned int vbo_id_;
};