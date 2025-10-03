// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "rectangle.hpp"
#include "config.hpp"

namespace fre2d {
namespace detail::circle {
static constexpr GLfloat default_thickness { 1.f };
static constexpr auto default_vertex =
fre2d_default_glsl_version
fre2d_default_buffer_layouts
R"(
out vec2 TexCoords;
out vec4 Color;
out vec2 Position;
out vec2 FragPos;
)"
fre2d_default_uniforms
R"(
void main() {
  gl_Position = Projection * View * Model * vec4(attr_Position, 0.f, 1.f);
  /* this will avoid unnecessary if statement for FlipVertically and FlipHorizontally */
)"
  fre2d_default_tex_coords
R"(
  Color = attr_Color;
  Position = attr_Position.xy;
  FragPos = vec2(Model * vec4(attr_Position, 0.f, 1.f));
}
)";

static constexpr auto default_fragment =
R"(#version 450 core

in vec2 TexCoords;
in vec4 Color;
in vec2 Position;
in vec2 FragPos;

out vec4 FragColor;

/* those uniforms are automatically passed by fre2d */
uniform sampler2D TextureSampler;
uniform bool UseTexture;
uniform float Thickness;
)"
fre2d_default_lighting_fragment
fre2d_default_color_func
R"(
void main() {
  vec4 default_color = calculate_color(Color, TextureSampler, TexCoords, UseTexture);
  vec2 local_pos = Position * 2.f;
  float distance = 1.f - length(local_pos);
  if(distance < 0.f || distance > Thickness) {
    discard;
  }

  FragColor = calculate_ambient_light(global_ambient_light);

  for(int i = 0; i < point_lights.length(); i++) {
    FragColor += mix(
      vec4(calculate_point_light(point_lights[i], TextureSampler, TexCoords, FragPos), default_color.a),
      vec4(0.f, 0.f, 0.f, 0.f),
      float(point_lights[i].disabled)
    );
  }

  FragColor *= default_color;
}
)";
} // namespace fre2d::detail::circle

class Circle : public Rectangle {
public:
  Circle() noexcept;

  explicit Circle(
    GLsizei width,
    GLsizei height,
    const glm::vec2& position,
    const glm::vec4& color,
    GLfloat thickness,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  ~Circle() override = default;

  void initialize_circle(
      GLsizei width,
      GLsizei height,
      const glm::vec2& position,
      const glm::vec4& color,
      GLfloat thickness,
      const Texture& texture = Texture::get_default_texture(),
      GLfloat rotation_rads = detail::drawable::default_rotation_radians,
      bool flip_vertically = detail::drawable::default_flip_vertically,
      bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void before_draw_custom(const Shader& shader,
                          const std::unique_ptr<Renderer>& rnd) noexcept override;

  void before_draw_custom(const Shader& shader,
                          const std::unique_ptr<Camera>& cam,
                          const std::unique_ptr<LightManager>& lm) noexcept override;

  [[nodiscard]] const GLfloat& get_thickness() const noexcept;
  void set_thickness(GLfloat thickness) noexcept;
protected:
  GLfloat _thickness;
};
} // namespace fre2d