// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define UNIFORM_LOC() this->get_uniform_location(uniform_name)

namespace fre2d {
Shader::Shader() noexcept {
  this->_program_id = std::make_shared<GLuint>(0);
}

Shader::Shader(const char* vertex_shader, const char* fragment_shader) noexcept {
  this->_program_id = std::make_shared<GLuint>(0);
  this->initialize(vertex_shader, fragment_shader);
}

Shader::Shader(GLuint program_id) noexcept
  : _program_id{std::make_shared<GLuint>(program_id)} {
}

Shader::~Shader() noexcept {
  this->release();
}

void Shader::initialize(const char* vertex_shader, const char* fragment_shader) noexcept {
  // probably enough for most cases
  char error_log[detail::shader::info_log_size];
  GLint success;

  GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_id, 1, &vertex_shader, NULL);
  glCompileShader(vertex_id);
  glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_id, detail::shader::info_log_size, NULL,
                       error_log);
    std::cerr << "fre2d error: vertex shader compilation failed (" << vertex_id
              << " " << error_log << ")\n";
    // TODO: vertex shader compilation failed; use custom log, use colorized.
  }

  GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_id, 1, &fragment_shader, NULL);
  glCompileShader(fragment_id);
  glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_id, detail::shader::info_log_size, NULL,
                       error_log);
    std::cerr << "fre2d error: fragment shader compilation failed ("
              << fragment_id << " " << error_log << ")\n";
    // TODO: fragment shader compilation failed; use custom log, use colorized.
  }

  *this->_program_id = glCreateProgram();
  glAttachShader(this->get_program_id(), vertex_id);
  glAttachShader(this->get_program_id(), fragment_id);
  glLinkProgram(this->get_program_id());
  glGetProgramiv(this->get_program_id(), GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->get_program_id(), detail::shader::info_log_size,
                        NULL, error_log);
    std::cerr << "fre2d error: shader program linking failed ("
              << this->get_program_id() << " " << error_log << ")\n";
    // TODO: shader program link stage failed; use custom log, use colorized.
  }

  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);
}

[[nodiscard]] const GLuint& Shader::get_program_id() const noexcept {
  return *this->_program_id;
}

[[nodiscard]] GLuint Shader::get_uniform_location(const char* uniform_name) const noexcept {
  return glGetUniformLocation(this->get_program_id(), uniform_name);
}

void Shader::use() const noexcept {
  glUseProgram(*this->_program_id);
}

void Shader::load(GLuint program_id) noexcept {
  this->release();
  this->_program_id = std::make_shared<GLuint>(program_id);
}

void Shader::load_override(GLuint program_id) noexcept {
  this->_program_id = std::make_shared<GLuint>(program_id);
}

void Shader::release() noexcept {
  // delete program when shader goes out of scope.
  if(this->get_program_id() != 0 && this->_program_id.use_count() == 1) {
    glDeleteProgram(*this->_program_id);
    *this->_program_id = 0;
  }
}

void Shader::set_bool(const char* uniform_name, GLboolean value) const noexcept {
  this->set_int(uniform_name, value);
}

void Shader::set_int(const char* uniform_name, GLint value) const noexcept {
  glProgramUniform1i(this->get_program_id(), UNIFORM_LOC(), value);
}

void Shader::set_uint(const char* uniform_name, GLuint value) const noexcept {
  glProgramUniform1ui(this->get_program_id(), UNIFORM_LOC(), value);
}

void Shader::set_float(const char* uniform_name, GLfloat value) const noexcept {
  glProgramUniform1f(this->get_program_id(), UNIFORM_LOC(), value);
}

void Shader::set_double(const char* uniform_name, GLdouble value) const noexcept {
  glProgramUniform1d(this->get_program_id(), UNIFORM_LOC(), value);
}

void Shader::set_bool_vec2(const char* uniform_name, const glm::bvec2& value) const noexcept {
  this->set_int_vec2(uniform_name, value);
}

void Shader::set_bool_vec3(const char* uniform_name, const glm::bvec3& value) const noexcept {
  this->set_int_vec3(uniform_name, value);
}

void Shader::set_bool_vec4(const char* uniform_name, const glm::bvec4& value) const noexcept {
  this->set_int_vec4(uniform_name, value);
}

void Shader::set_int_vec2(const char* uniform_name, const glm::ivec2& value) const noexcept {
  glProgramUniform2iv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_int_vec3(const char* uniform_name, const glm::ivec3& value) const noexcept {
  glProgramUniform3iv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_int_vec4(const char* uniform_name, const glm::ivec4& value) const noexcept {
  glProgramUniform4iv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_uint_vec2(const char* uniform_name, const glm::uvec2& value) const noexcept {
  glProgramUniform2uiv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_uint_vec3(const char* uniform_name, const glm::uvec3& value) const noexcept {
  glProgramUniform3uiv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_uint_vec4(const char* uniform_name, const glm::uvec4& value) const noexcept {
  glProgramUniform4uiv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_float_vec2(const char* uniform_name, const glm::fvec2& value) const noexcept {
  glProgramUniform2fv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_float_vec3(const char* uniform_name, const glm::fvec3& value) const noexcept {
  glProgramUniform3fv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_float_vec4(const char* uniform_name, const glm::fvec4& value) const noexcept {
  glProgramUniform4fv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_double_vec2(const char* uniform_name, const glm::dvec2& value) const noexcept {
  glProgramUniform2dv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_double_vec3(const char* uniform_name, const glm::dvec3& value) const noexcept {
  glProgramUniform3dv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_double_vec4(const char* uniform_name, const glm::dvec4& value) const noexcept {
  glProgramUniform4dv(this->get_program_id(), UNIFORM_LOC(), 1, glm::value_ptr(value));
}

void Shader::set_float_mat2x2(const char* uniform_name, const glm::f32mat2x2& value) const noexcept {
  glProgramUniformMatrix2fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat2x3(const char* uniform_name, const glm::f32mat2x3& value) const noexcept {
  glProgramUniformMatrix2x3fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat2x4(const char* uniform_name, const glm::f32mat2x4& value) const noexcept {
  glProgramUniformMatrix2x4fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat3x2(const char* uniform_name, const glm::f32mat3x2& value) const noexcept {
  glProgramUniformMatrix3x2fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat3x3(const char* uniform_name, const glm::f32mat3x4& value) const noexcept {
  glProgramUniformMatrix3fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat4x2(const char* uniform_name, const glm::f32mat4x2& value) const noexcept {
  glProgramUniformMatrix4x2fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat4x3(const char* uniform_name, const glm::f32mat4x3& value) const noexcept {
  glProgramUniformMatrix4x3fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_float_mat4x4(const char* uniform_name, const glm::f32mat4x4& value) const noexcept {
  glProgramUniformMatrix4fv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat2x2(const char* uniform_name, const glm::f64mat2x2& value) const noexcept {
  glProgramUniformMatrix2dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat2x3(const char* uniform_name, const glm::f64mat2x3& value) const noexcept {
  glProgramUniformMatrix2x3dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat2x4(const char* uniform_name, const glm::f64mat2x4& value) const noexcept {
  glProgramUniformMatrix2x4dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat3x2(const char* uniform_name, const glm::f64mat3x2& value) const noexcept {
  glProgramUniformMatrix3x2dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat3x3(const char* uniform_name, const glm::f64mat3x4& value) const noexcept {
  glProgramUniformMatrix3dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat4x2(const char* uniform_name, const glm::f64mat4x2& value) const noexcept {
  glProgramUniformMatrix4x2dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat4x3(const char* uniform_name, const glm::f64mat4x3& value) const noexcept {
  glProgramUniformMatrix4x3dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_double_mat4x4(const char* uniform_name, const glm::f64mat4x4& value) const noexcept {
  glProgramUniformMatrix4dv(this->get_program_id(), UNIFORM_LOC(), 1, GL_FALSE, glm::value_ptr(value));
}
} // namespace fre2d

#undef this->get_program_id(), UNIFORM_LOC