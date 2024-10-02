// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <vertex_buffer.hpp>
#include <framebuffer.hpp>
#include <error.hpp>

namespace fre2d {
// use this to initialize VertexBuffer later.
VertexBuffer::VertexBuffer() noexcept : _vbo_id{0} {}

// use this to initialize VertexBuffer at the time.
VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices) noexcept
  : _vbo_id{0} {
  this->initialize(vertices);
}

VertexBuffer::~VertexBuffer() noexcept {
  if(this->get_vbo_id() != 0) {
    glDeleteBuffers(1, &this->_vbo_id);
  }
}

void VertexBuffer::bind() const noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_id);
}

void VertexBuffer::unbind() const noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::initialize(const std::vector<Vertex> &vertices) noexcept {
  glGenBuffers(1, &this->_vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_id);
  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * sizeof(Vertex),
    vertices.data(),
    GL_STATIC_DRAW
  );
}

// initialize empty vertex buffer.
void VertexBuffer::empty_initialize(GLsizei size) noexcept {
  glGenBuffers(1, &this->_vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, this->_vbo_id);
  // TODO: support custom usage flags like GL_STATIC_DRAW, GL_DYNAMIC_DRAW
  glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

[[nodiscard]] const GLuint &VertexBuffer::get_vbo_id() const noexcept {
  return this->_vbo_id;
}
} // namespace fre2d