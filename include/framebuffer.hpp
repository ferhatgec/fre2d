// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "renderbuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include <glad/glad.h>
#include <concepts>

namespace fre2d {
namespace detail::framebuffer {
static constexpr auto default_vertex =
R"(#version 450 core
layout (location = 0) in vec3 attr_Position;
layout (location = 1) in vec4 attr_Color;
layout (location = 2) in vec2 attr_TexCoords;
out vec2 TexCoords;
out vec4 Color;
void main() {
  gl_Position = vec4(attr_Position, 1.f);
  TexCoords = attr_TexCoords;
  Color = attr_Color;
}
)";

static constexpr auto default_fragment =
R"(#version 450 core
in vec2 TexCoords;
in vec4 Color;
out vec4 FragColor;
uniform sampler2D ScreenTexture;
void main() {
  FragColor = texture(ScreenTexture, TexCoords) * Color;
})";

static constexpr std::array<Vertex, 6> framebuffer_vertices {
  Vertex{{-1.0f, 1.0f}, detail::vertex::default_color, {0.f, 1.f}},
  Vertex{{-1.0f, -1.0f}, detail::vertex::default_color, {0.f, 0.f}},
  Vertex{{1.0f, -1.0f}, detail::vertex::default_color, {1.f, 0.f}},
  Vertex{{-1.0f, 1.0f}, detail::vertex::default_color, {0.f, 1.f}},
  Vertex{{1.0f, -1.0f}, detail::vertex::default_color, {1.f, 0.f}},
  Vertex{{1.0f, 1.0f}, detail::vertex::default_color, {1.f, 1.f}}
};
} // namespace fre2d::detail::framebuffer
// TODO: support multisampling for antialiasing.
class Framebuffer {
public:
  Framebuffer(); // then use initialize().
  explicit Framebuffer(
    GLsizei width,
    GLsizei height,
    bool use_default = false,
    const char* default_vertex_shader = detail::framebuffer::default_vertex,
    const char* default_fragment_shader = detail::framebuffer::default_fragment
  ) noexcept;
  ~Framebuffer() noexcept;

  // binds current framebuffer (can be default framebuffer) and resize with current size.
  void bind() noexcept;

  void initialize(
    GLsizei width,
    GLsizei height,
    bool use_default = false,
    const char* default_vertex_shader = detail::framebuffer::default_vertex,
    const char* default_fragment_shader = detail::framebuffer::default_fragment
  ) noexcept;

  // unbinds current framebuffer to default one.
  void unbind() noexcept;
  void clear() const noexcept;

  // to render custom framebuffer,
  // default framebuffer will just set counter to 0.
  void render_texture() noexcept;

  void resize(GLsizei width, GLsizei height) noexcept; // use this everytime to resize your framebuffer
                                                       // (including default framebuffer. resize() will call glViewport directly)

  [[nodiscard]] const GLuint& get_fbo_id() const noexcept;
  [[nodiscard]] bool is_complete() const noexcept;

  [[nodiscard]] const GLsizei& get_width() const noexcept;
  [[nodiscard]] const GLsizei& get_height() const noexcept;

  [[nodiscard]] const Texture& get_color_buffer() const noexcept;
  [[nodiscard]] const Renderbuffer& get_depth_and_stencil_renderbuffer() const noexcept;

  [[nodiscard]] const VertexArray& get_framebuffer_vao() const noexcept;
  [[nodiscard]] const VertexBuffer& get_framebuffer_vbo() const noexcept;
  [[nodiscard]] const Shader& get_framebuffer_shader() const noexcept;

  template<typename Callable, typename... Args>
  requires std::invocable<Callable, Args...>
  void call(Callable&& fn, Args&&... args) noexcept {
    this->bind();
    fn(std::forward<Args>(args)...);
    this->unbind();
  }

  void clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) noexcept;
  static void clear_color_force(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) noexcept;
private:
  Texture _color_buffer;
  Renderbuffer _depth_and_stencil_rb;
  Shader _shader;
  VertexArray _fb_vao;
  VertexBuffer _fb_vbo;
  GLuint _fbo_id;
  GLsizei _width;
  GLsizei _height;

  bool _first_time;
  bool _clear_called;
};
} // namespace fre2d