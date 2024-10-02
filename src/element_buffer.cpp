// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <element_buffer.hpp>
#include <error.hpp>

namespace fre2d {
ElementBuffer::ElementBuffer(const std::vector<GLuint>& indices, bool initialize) noexcept
  : _ebo_id{0}, _indices_count{0} {
  if(initialize) {
    this->initialize(indices);
  }
}

ElementBuffer::~ElementBuffer() noexcept {
  if(this->get_ebo_id() != 0) {
    this->unbind();
    glDeleteBuffers(1, &this->_ebo_id);
  }
}

void ElementBuffer::bind() const noexcept {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo_id);
}

void ElementBuffer::unbind() const noexcept {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::initialize(const std::vector<GLuint> &indices) noexcept {
  glGenBuffers(1, &this->_ebo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  this->_indices_count = static_cast<GLsizei>(indices.size());
}

[[nodiscard]] const GLuint& ElementBuffer::get_ebo_id() const noexcept {
  return this->_ebo_id;
}

[[nodiscard]] const GLsizei& ElementBuffer::get_indices_count() const noexcept {
  return this->_indices_count;
}
} // namespace fre2d