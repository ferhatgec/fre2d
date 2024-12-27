// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <circle.hpp>
#include <helper_funcs.hpp>

namespace fre2d {
Circle::Circle() noexcept
  : _thickness{detail::circle::default_thickness} {
}

Circle::Circle(
  GLsizei width,
  GLsizei height,
  const glm::vec2& position,
  const glm::vec4& color,
  GLfloat thickness,
  const Texture& texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->initialize_circle(
    width,
    height,
    position,
    color,
    thickness,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Circle::initialize_circle(
  GLsizei width,
  GLsizei height,
  const glm::vec2& position,
  const glm::vec4& color,
  GLfloat thickness,
  const Texture& texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->initialize_rectangle(
    width,
    height,
    position,
    color,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
  this->_thickness = thickness;
}

void Circle::before_draw_custom(
  const Shader& shader,
  const std::unique_ptr<Camera>& camera
) noexcept {
  shader.set_float("Thickness", this->_thickness);
}

[[nodiscard]] const GLfloat& Circle::get_thickness() const noexcept {
  return this->_thickness;
}

void Circle::set_thickness(GLfloat thickness) noexcept {
  this->_thickness = (thickness > 1.f) ? 1.f : (thickness < 0.f ? 0.f : thickness);
}
} // namespace fre2d