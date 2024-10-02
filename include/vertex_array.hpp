// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>

namespace fre2d {
class ElementBuffer;
}
namespace fre2d {
class VertexBuffer;
}
namespace fre2d {
namespace detail::vertex_array {
static constexpr bool initialize_now { false };
} // namespace fre2d::detail

class VertexArray {
public:
  friend class Label;

  explicit VertexArray(bool initialize = detail::vertex_array::initialize_now) noexcept;
  ~VertexArray() noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;
  void initialize() noexcept;

  [[nodiscard]] const GLuint& get_vao_id() const noexcept;
private:
  GLuint _vao_id;
};
} // namespace fre2d