// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <ssbo.hpp>

namespace fre2d {
SSBO::SSBO() noexcept : _ssbo_id{0}, _binding_id{0}
{}

template <typename T>
SSBO::SSBO(GLint binding, const std::vector<T> &buffer) noexcept {
  this->initialize(binding, buffer);
}

SSBO::~SSBO() noexcept {
  if(this->get_ssbo_id() != 0) {
    glDeleteBuffers(1, &this->_ssbo_id);
  }
}

void SSBO::bind() const noexcept {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->get_ssbo_id());
}

void SSBO::unbind() const noexcept {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

[[nodiscard]] const GLuint &SSBO::get_ssbo_id() const noexcept {
  return this->_ssbo_id;
}

void SSBO::empty_initialize(GLint binding) noexcept {
  glGenBuffers(1, &this->_ssbo_id);
  this->bind();
  glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, this->get_ssbo_id());
  this->_binding_id = binding;
  this->unbind();
}

template <typename T>
void SSBO::initialize(GLint binding, const std::vector<T>& buffer) noexcept {
  glGenBuffers(1, &this->_ssbo_id);
  this->bind();
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, this->get_ssbo_id());
  this->_binding_id = binding;
  this->unbind();
}
} // namespace fre2d