// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "drawable.hpp"
#include <memory>
#include <array>

namespace fre2d {
class Rectangle : public Drawable {
public:
  Rectangle() noexcept;

  explicit Rectangle(
    GLsizei width,
    GLsizei height,
    const glm::vec2& position,
    const std::array<glm::vec4, 4>& color,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  explicit Rectangle(
    GLsizei width,
    GLsizei height,
    const glm::vec2& position,
    const glm::vec4& color,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  ~Rectangle() override = default;

  void initialize_rectangle(
    GLsizei width,
    GLsizei height,
    const glm::vec2& position,
    const std::array<glm::vec4, 4>& color,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void initialize_rectangle(
    GLsizei width,
    GLsizei height,
    const glm::vec2& position,
    const glm::vec4& color,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void draw(const Shader &shader, const std::unique_ptr<Renderer>& rnd) noexcept override;
private:
  std::vector<Vertex> _vertices;
};
} // namespace fre2d