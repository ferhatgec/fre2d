// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "font.hpp"
#include "drawable.hpp"
#include <string>
#include <variant>

namespace fre2d {
namespace detail::label {
static constexpr std::array default_colors {
  drawable::default_color,
  drawable::default_color,
  drawable::default_color,
  drawable::default_color,
  drawable::default_color,
  drawable::default_color
};

static constexpr auto default_vertex =
R"(#version 450 core
layout (location = 0) in vec2 attr_Position;
layout (location = 1) in vec4 attr_Color;
layout (location = 2) in vec2 attr_TexCoords;

out vec4 attr_TextColor;
out vec2 TexCoords;

/* those uniforms are automatically passed by fre2d */
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
  gl_Position = Projection * View * Model * vec4(attr_Position, 0.f, 1.f);
  TexCoords = attr_TexCoords;
  attr_TextColor = attr_Color;
}
)";

static constexpr auto default_fragment =
R"(#version 450 core

in vec4 attr_TextColor;
in vec2 TexCoords;

/* this over gl_FragColor gives us flexibility of manipulating it easily */
out vec4 Color;

uniform sampler2D Text;
uniform vec4 TextColor;

void main() {
  vec4 sampled = vec4(1.f, 1.f, 1.f, texture(Text, TexCoords).r);
  Color = TextColor * attr_TextColor * sampled;
}
)";
} // namespace fer2d::detail::label

class Label : public Drawable {
public:
  Label() noexcept;
  Label(
    const Font& font,
    const char* text,
    const glm::vec2& position,
    const glm::vec4& color = detail::drawable::default_color,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians
  ) noexcept;
  Label(
    const Font& font,
    const char* text,
    const glm::vec2& position,
    const std::array<glm::vec4, 6>& colors,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians
  ) noexcept;
  ~Label() override;

  void initialize_label(
    const Font& font,
    const char* text,
    const glm::vec2& position,
    const glm::vec4& color = detail::drawable::default_color,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians
  ) noexcept;

  void initialize_label(
    const Font& font,
    const char* text,
    const glm::vec2& position,
    const std::array<glm::vec4, 6>& colors = detail::label::default_colors,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians
  ) noexcept;

  void draw(const Shader &shader, const std::unique_ptr<Camera>& camera) noexcept override;
  void before_draw(const Shader& shader, const std::unique_ptr<Camera>& camera) noexcept override;

  [[nodiscard]] const std::string& get_current_text() const noexcept;
private:
  void _initialize_fields_other_than_color(
    const Font& font,
    const char* text,
    const glm::vec2& position,
    GLfloat rotation_rads
  ) noexcept;

  Font _font;
  VertexArray _vao;
  VertexBuffer _vbo;
  std::string _text;

  GLfloat _bbox_w;
  GLfloat _bbox_h;

  std::variant<std::array<glm::vec4, 6>, glm::vec4> _colors;
};
} // namespace fre2d