#pragma once

#include <vector>

class VertexBufferLayout {
 public:
  enum gl_type { float_ = GL_FLOAT, int_ = GL_INT };
  VertexBufferLayout() = default;

  void addAttribute(gl_type gl_type, size_t elements) {
    // TODO check if GL
    attributes_types_.push_back(gl_type);
    attribute_num_elements_.push_back(elements);
    switch (gl_type) {
      case (gl_type::float_):
        attribute_type_size_.push_back(sizeof(float));
        break;

      case (gl_type::int_):
        attribute_type_size_.push_back(sizeof(int));
        break;
    }
  }

  const std::vector<unsigned int>& getAttributeType() const { return attributes_types_; }
  const std::vector<unsigned int>& getAttributeNumElements() const { return attribute_num_elements_; }
  const std::vector<unsigned int>& getAttributeTypeSize() const { return attribute_type_size_; }

  unsigned int getNumAttributes() const { return attributes_types_.size(); }

  static unsigned int getsizeFromType(gl_type type) {
    switch (type) {
      case gl_type::float_:
        return sizeof(float);

      case gl_type::int_:
        return sizeof(int);
        break;
    }
  }

  static unsigned int getsizeFromType(unsigned int GL_TYPE_) {
    switch (GL_TYPE_) {
      case GL_FLOAT:
        return sizeof(float);

      case GL_INT:
        return sizeof(int);
        break;
      default:
        throw std::runtime_error("GL TYPE Not supported");
    }
  }

 private:
  std::vector<unsigned int> attributes_types_;
  std::vector<unsigned int> attribute_num_elements_;
  std::vector<unsigned int> attribute_type_size_;
};