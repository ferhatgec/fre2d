// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>
#include <vector>

namespace fre2d {
class SSBO {
public:
  SSBO() noexcept;

  template<typename T>
  explicit SSBO(GLint binding, const std::vector<T>& buffer) noexcept;

  ~SSBO() noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

  [[nodiscard]] const GLuint& get_ssbo_id() const noexcept;

  void empty_initialize(GLint binding) noexcept;

  template<typename T>
  void initialize(GLint binding, const std::vector<T>& buffer) noexcept;
private:
  GLuint _ssbo_id;
  GLint _binding_id;
};
} // namespace fre2d