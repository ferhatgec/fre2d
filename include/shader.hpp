// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace fre2d {
namespace detail::shader {
// default shaders are hardcoded, i think it's good approach;
// since not messing with file paths are huge plus and 0.0000001s runtime speed!
// TODO: we might use preprocessor macros instead of relying on runtime booleans
static constexpr auto default_vertex =
R"(#version 450 core
layout (location = 0) in vec3 attr_Position;
layout (location = 1) in vec4 attr_Color;
layout (location = 2) in vec2 attr_TexCoords;

out vec2 TexCoords;
out vec4 Color;

/* those uniforms are automatically passed by fre2d */
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
  gl_Position = Projection * View * Model * vec4(attr_Position, 1.f);
  TexCoords = attr_TexCoords.xy;
  Color = attr_Color;
}
)";

static constexpr auto default_fragment =
R"(#version 450 core

in vec2 TexCoords;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D TextureSampler;
uniform bool UseTexture;

void main() {
  /* if UseTexture = 0.f, it will return Color.
     if UseTexture = 1.f, then it will return Color * texture(TextureSampler, TexCoords) */
  FragColor = mix(Color, Color * texture(TextureSampler, TexCoords), float(UseTexture));
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
  ~Shader() noexcept;

  void initialize(
    const char* vertex_shader = detail::shader::default_vertex,
    const char* fragment_shader = detail::shader::default_fragment
  ) noexcept;
  [[nodiscard]] const GLuint& get_program_id() const noexcept;
  [[nodiscard]] GLuint get_uniform_location(const char* uniform_name) const noexcept;

  void use() const noexcept;

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
  GLuint _program_id;
};
} // namespace fre2d