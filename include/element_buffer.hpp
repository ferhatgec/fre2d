// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <vector>
#include <glad/glad.h>

namespace fre2d {
namespace detail::element_buffer {
static constexpr bool initialize_now { false };
} // namespace fre2d::detail::element_buffer

class ElementBuffer {
public:
  explicit ElementBuffer(const std::vector<GLuint>& indices = {},
                         bool initialize = detail::element_buffer::initialize_now) noexcept;
  ~ElementBuffer() noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;
  void initialize(const std::vector<GLuint>& indices) noexcept;

  [[nodiscard]] const GLuint& get_ebo_id() const noexcept;
  [[nodiscard]] const GLsizei& get_indices_count() const noexcept;
private:
  GLuint _ebo_id;
  GLsizei _indices_count;
};
} // namespace fre2d