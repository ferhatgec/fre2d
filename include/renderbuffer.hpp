// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>

namespace fre2d {
namespace detail::renderbuffer {
static constexpr GLuint default_internal_format { GL_DEPTH24_STENCIL8 };
static constexpr GLuint default_attachment { GL_DEPTH_STENCIL_ATTACHMENT };
} // namespace fre2d::detail::renderbuffer

class Framebuffer;
class Renderbuffer {
public:
  Renderbuffer() = default; // then use initialize()
  explicit Renderbuffer(GLsizei width,
                        GLsizei height,
                        GLuint internal_format = detail::renderbuffer::default_internal_format,
                        GLuint attachment = detail::renderbuffer::default_attachment) noexcept;
  ~Renderbuffer() noexcept;

  void initialize(GLsizei width,
                  GLsizei height,
                  GLuint internal_format = detail::renderbuffer::default_internal_format,
                  GLuint attachment = detail::renderbuffer::default_attachment) noexcept;
  void bind() const noexcept;
  void unbind() const noexcept;
  void attach(const Framebuffer& fb, GLuint attachment = detail::renderbuffer::default_attachment) const noexcept;

  [[nodiscard]] const GLuint& get_rbo_id() const noexcept;
private:
  GLuint _rbo_id;
  GLsizei _width;
  GLsizei _height;
  GLuint _internal_format;
  GLuint _attachment;
};
} // namespace fre2d