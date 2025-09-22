// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "config.hpp"
#include "light.hpp"

namespace fre2d {
namespace detail::shader {
// default shaders are hardcoded, i think it's good approach;
// since not messing with file paths are huge plus and 0.0000001s runtime speed!
// TODO: we might use preprocessor macros instead of relying on runtime booleans
static constexpr auto default_vertex =
fre2d_default_glsl_version
fre2d_default_buffer_layouts
R"(
out vec2 TexCoords;
out vec4 Color;
out vec2 FragPos;
)"
fre2d_default_uniforms
R"(
void main() {
  gl_Position = Projection * View * Model * vec4(attr_Position, 0.f, 1.f);
  FragPos = vec2(Model * vec4(attr_Position, 0.f, 1.f));
)"
  fre2d_default_tex_coords
R"(
  Color = attr_Color;
}
)";

static constexpr auto default_fragment =
R"(#version 450 core

in vec2 TexCoords;
in vec4 Color;
in vec2 FragPos;

out vec4 FragColor;

uniform sampler2D TextureSampler;
uniform bool UseTexture;
)"
fre2d_default_point_light_fragment
fre2d_default_color_func
R"(
void main() {
  vec4 default_color = calculate_color(Color, TextureSampler, TexCoords, UseTexture);
  /* if UseTexture = 0.f, it will return Color.
     if UseTexture = 1.f, then it will return Color * texture(TextureSampler, TexCoords) */
  FragColor = default_color;
  for(int i = 0; i < point_lights.length(); i++) {
    FragColor += vec4(calculate_point_light(point_lights[i], TextureSampler, TexCoords, FragPos), default_color.a);
  }
})";

static constexpr auto info_log_size { 512 };
static constexpr bool initialize_now { true };
} // namespace fre2d::detail::shader

class Shader {
public:
  Shader() noexcept;

  // currently fre2d does not support reading from file,
  // it's easy to implement but idk. you can use your own approach to read file,
  // then pass it to ctor here.
  Shader(const char* vertex_shader,
         const char* fragment_shader) noexcept;
  Shader(GLuint program_id) noexcept;
  ~Shader() noexcept;

  void initialize(
    const char* vertex_shader = detail::shader::default_vertex,
    const char* fragment_shader = detail::shader::default_fragment
  ) noexcept;

  [[nodiscard]] const GLuint& get_program_id() const noexcept;
  [[nodiscard]] GLuint get_uniform_location(const char* uniform_name) const noexcept;

  void use() const noexcept;

  void load(GLuint program_id) noexcept;
  void load_override(GLuint program_id) noexcept;
  void release() noexcept;

  // we might use just overload `set`, but this is more descriptive imo.
  void set_bool(const char* uniform_name, GLboolean value) const noexcept;
  void set_int(const char* uniform_name, GLint value) const noexcept;
  void set_uint(const char* uniform_name, GLuint value) const noexcept;
  void set_float(const char* uniform_name, GLfloat value) const noexcept;
  void set_double(const char* uniform_name, GLdouble value) const noexcept;

  void set_bool_vec2(const char* uniform_name, const glm::bvec2& value) const noexcept;
  void set_bool_vec3(const char* uniform_name, const glm::bvec3& value) const noexcept;
  void set_bool_vec4(const char* uniform_name, const glm::bvec4& value) const noexcept;

  void set_int_vec2(const char* uniform_name, const glm::ivec2& value) const noexcept;
  void set_int_vec3(const char* uniform_name, const glm::ivec3& value) const noexcept;
  void set_int_vec4(const char* uniform_name, const glm::ivec4& value) const noexcept;

  void set_uint_vec2(const char* uniform_name, const glm::uvec2& value) const noexcept;
  void set_uint_vec3(const char* uniform_name, const glm::uvec3& value) const noexcept;
  void set_uint_vec4(const char* uniform_name, const glm::uvec4& value) const noexcept;

  // yes, OpenGL uses float by default; so we might do not need to mention 'float',
  // however it's good to declare them explicitly, why not?
  void set_float_vec2(const char* uniform_name, const glm::fvec2& value) const noexcept;
  void set_float_vec3(const char* uniform_name, const glm::fvec3& value) const noexcept;
  void set_float_vec4(const char* uniform_name, const glm::fvec4& value) const noexcept;

  void set_double_vec2(const char* uniform_name, const glm::dvec2& value) const noexcept;
  void set_double_vec3(const char* uniform_name, const glm::dvec3& value) const noexcept;
  void set_double_vec4(const char* uniform_name, const glm::dvec4& value) const noexcept;

  void set_float_mat2x2(const char* uniform_name, const glm::f32mat2x2& value) const noexcept;
  void set_float_mat2x3(const char* uniform_name, const glm::f32mat2x3& value) const noexcept;
  void set_float_mat2x4(const char* uniform_name, const glm::f32mat2x4& value) const noexcept;
  void set_float_mat3x2(const char* uniform_name, const glm::f32mat3x2& value) const noexcept;
  void set_float_mat3x3(const char* uniform_name, const glm::f32mat3x4& value) const noexcept;
  void set_float_mat4x2(const char* uniform_name, const glm::f32mat4x2& value) const noexcept;
  void set_float_mat4x3(const char* uniform_name, const glm::f32mat4x3& value) const noexcept;
  void set_float_mat4x4(const char* uniform_name, const glm::f32mat4x4& value) const noexcept;

  void set_double_mat2x2(const char* uniform_name, const glm::f64mat2x2& value) const noexcept;
  void set_double_mat2x3(const char* uniform_name, const glm::f64mat2x3& value) const noexcept;
  void set_double_mat2x4(const char* uniform_name, const glm::f64mat2x4& value) const noexcept;
  void set_double_mat3x2(const char* uniform_name, const glm::f64mat3x2& value) const noexcept;
  void set_double_mat3x3(const char* uniform_name, const glm::f64mat3x4& value) const noexcept;
  void set_double_mat4x2(const char* uniform_name, const glm::f64mat4x2& value) const noexcept;
  void set_double_mat4x3(const char* uniform_name, const glm::f64mat4x3& value) const noexcept;
  void set_double_mat4x4(const char* uniform_name, const glm::f64mat4x4& value) const noexcept;
private:
  std::shared_ptr<GLuint> _program_id;
};
} // namespace fre2d