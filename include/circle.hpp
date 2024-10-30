// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "rectangle.hpp"

namespace fre2d {
namespace detail::circle {
static constexpr GLfloat default_thickness { 1.f };
static constexpr auto default_vertex = R"(
#version 450 core
layout (location = 0) in vec3 attr_Position;
layout (location = 1) in vec4 attr_Color;
layout (location = 2) in vec2 attr_TexCoords;

out vec2 TexCoords;
out vec4 Color;
out vec2 Position;

/* those uniforms are automatically passed by fre2d */
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
  gl_Position = Projection * View * Model * vec4(attr_Position, 1.f);
  TexCoords = attr_TexCoords.xy;
  Color = attr_Color;
  Position = attr_Position.xy;
}
)";

static constexpr auto default_fragment = R"(
#version 450 core

in vec2 TexCoords;
in vec4 Color;
in vec2 Position;

out vec4 FragColor;

/* those uniforms are automatically passed by fre2d */
uniform sampler2D TextureSampler;
uniform bool UseTexture;
uniform float Thickness;

void main() {
  vec2 local_pos = Position * 2.f;
  float distance = 1.f - length(local_pos);
  if(distance < 0.f || distance > Thickness) {
    discard;
  }
  FragColor = mix(Color, Color * texture(TextureSampler, TexCoords), float(UseTexture));
}
)";
} // namespace fre2d::detail::circle

class Circle : public Rectangle {
public:
  Circle() noexcept;

  explicit Circle(GLsizei width,
                  GLsizei height,
                  const glm::vec2& position,
                  const glm::vec4& color,
                  GLfloat thickness,
                  const Texture& texture = Texture::get_default_texture(),
                  GLfloat rotation_rads =
                      detail::drawable::default_rotation_radians) noexcept;

  ~Circle() override = default;

  void initialize_circle(
      GLsizei width,
      GLsizei height,
      const glm::vec2& position,
      const glm::vec4& color,
      GLfloat thickness,
      const Texture& texture = Texture::get_default_texture(),
      GLfloat rotation_rads =
          detail::drawable::default_rotation_radians) noexcept;

  void before_draw_custom(const Shader& shader,
                           const std::unique_ptr<Camera>& camera) noexcept override;

  [[nodiscard]] const GLfloat& get_thickness() const noexcept;
  void set_thickness(GLfloat thickness) noexcept;
protected:
  GLfloat _thickness;
};
} // namespace fre2d