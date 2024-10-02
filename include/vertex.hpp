// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glm/glm.hpp>

namespace fre2d {
namespace detail::vertex {
static constexpr auto default_position = glm::vec2(0.f, 0.f);
static constexpr auto default_color = glm::vec4(1.f, 1.f, 1.f, 1.f);
static constexpr auto default_tex_coord = glm::vec2(0.f, 0.f);
} // namespace fre2d::detail::vertex

// contains no texture coordinates; used for Polygon class to explicitly use
// automatic texture assignation.
class Vertex2 {
public:
  constexpr Vertex2(
    const glm::vec2& position = detail::vertex::default_position,
    const glm::vec4& color = detail::vertex::default_color
  ) noexcept : _position{position}, _color{color} {}

  constexpr void set_position(const glm::vec2& position) noexcept {
    this->_position = position;
  }

  constexpr void set_color(const glm::vec4& color) noexcept {
    this->_color = color;
  }

  [[nodiscard]] constexpr const glm::vec2& get_position() const noexcept {
    return this->_position;
  }

  [[nodiscard]] constexpr const glm::vec4& get_color() const noexcept {
    return this->_color;
  }
private:
  glm::vec2 _position;
  glm::vec4 _color;
};

class Vertex {
public:
  constexpr Vertex(
    const glm::vec2& position = detail::vertex::default_position,
    const glm::vec4& color = detail::vertex::default_color,
    const glm::vec2& tex_coord = detail::vertex::default_tex_coord
  ) noexcept : _position{position}, _color{color}, _tex_coord{tex_coord} {}

  constexpr Vertex(
    const Vertex2& vert2,
    const glm::vec2& tex_coord = detail::vertex::default_tex_coord
  ) noexcept : _position{vert2.get_position()},
               _color{vert2.get_color()},
               _tex_coord{tex_coord} {}

  constexpr void set_position(const glm::vec2& position) noexcept {
    this->_position = position;
  }

  constexpr void set_color(const glm::vec4& color) noexcept {
    this->_color = color;
  }

  constexpr void set_tex_coord(const glm::vec2& tex_coord) noexcept {
    this->_tex_coord = tex_coord;
  }

  constexpr void set_vertex2(const Vertex2& vert2) noexcept {
    this->set_position(vert2.get_position());
    this->set_color(vert2.get_color());
  }

  [[nodiscard]] constexpr const glm::vec2& get_position() const noexcept {
    return this->_position;
  }

  [[nodiscard]] constexpr const glm::vec4& get_color() const noexcept {
    return this->_color;
  }

  [[nodiscard]] constexpr const glm::vec2& get_tex_coord() const noexcept {
    return this->_tex_coord;
  }
private:
  glm::vec2 _position;
  glm::vec4 _color;
  glm::vec2 _tex_coord;
};

using Vertex3 = Vertex;
} // namespace fre2d