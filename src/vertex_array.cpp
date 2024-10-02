// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <error.hpp>
#include <vertex_buffer.hpp>
#include <vertex_array.hpp>

namespace fre2d {
VertexArray::VertexArray(bool initialize) noexcept
  : _vao_id{0} {
  if(initialize) {
    this->initialize();
  }
}

VertexArray::~VertexArray() noexcept {
  if(this->get_vao_id() != 0) {
    this->unbind(); // it's possible to bind VAO, we ensure that no one bound to that VAO.
    glDeleteVertexArrays(1, &this->_vao_id);
  }
}

void VertexArray::bind() const noexcept {
  glBindVertexArray(this->_vao_id);
}

void VertexArray::unbind() const noexcept {
  glBindVertexArray(0);
}

void VertexArray::initialize() noexcept {
  if (this->_vao_id == 0) {
    glGenVertexArrays(1, &this->_vao_id);
    this->bind();
  }
}

[[nodiscard]] const GLuint& VertexArray::get_vao_id() const noexcept {
  return this->_vao_id;
}
} // namespace fre2d