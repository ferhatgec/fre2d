// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "vertex.hpp"
#include <glad/glad.h>
#include <vector>
#include <array>

namespace fre2d {
namespace detail::vertex_buffer {
static constexpr bool initialize_now { false };
static const std::vector<Vertex> default_vertices {};
static constexpr std::array<Vertex, 0> default_vertices_array {};
} // namespace fre2d::detail::vertex_buffer

class VertexBuffer {
public:
  friend class Label;

  VertexBuffer() noexcept;

  explicit VertexBuffer(
    const std::vector<Vertex>& vertices
  ) noexcept;

  // 6 is not a magic number; reserved for framebuffer rectangle (6 vertices)
  template<size_t N>
  explicit VertexBuffer(const std::array<Vertex, N>& vertices) noexcept
    : _vbo_id{0} {
    this->initialize<N>(vertices);
  }

  ~VertexBuffer() noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;
  void initialize(const std::vector<Vertex>& vertices) noexcept;

  void empty_initialize(GLsizei size) noexcept;

  template<size_t N>
  void initialize(const std::array<Vertex, N>& vertices) noexcept {
    glCreateBuffers(1, &this->_vbo_id);
    // TODO: support different usage flags
    glNamedBufferData(this->get_vbo_id(), sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
  }

  [[nodiscard]] const GLuint& get_vbo_id() const noexcept;
private:
  GLuint _vbo_id;
};
} // namespace fre2d