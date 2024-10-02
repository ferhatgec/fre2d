// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include "framebuffer.hpp"
#include <renderbuffer.hpp>

namespace fre2d {
// i don't know why but clang-tidy gives me "constructor does not initialize these fields: ...".
// we actually initialize them. it might be false positive.
// (i use clang-tidy that comes with CLion, so it might be fixed in later versions)
Renderbuffer::Renderbuffer(GLsizei width, GLsizei height, GLuint internal_format, GLuint attachment) noexcept {
  this->initialize(width, height, internal_format, attachment);
}

Renderbuffer::~Renderbuffer() noexcept {
  if(this->get_rbo_id() != 0) {
    glDeleteRenderbuffers(1, &this->_rbo_id);
  }
}

void Renderbuffer::initialize(GLsizei width, GLsizei height, GLuint internal_format, GLuint attachment) noexcept {
  this->_width = width;
  this->_height = height;
  this->_internal_format = internal_format;
  this->_attachment = attachment;
  glGenRenderbuffers(1, &this->_rbo_id);
  this->bind();
  glRenderbufferStorage(GL_RENDERBUFFER, this->_internal_format, width, height);
  this->unbind();
}

void Renderbuffer::bind() const noexcept {
  glBindRenderbuffer(GL_RENDERBUFFER, this->get_rbo_id());
}

void Renderbuffer::unbind() const noexcept {
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::attach(const Framebuffer& fb, GLuint attachment) const noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, fb.get_fbo_id());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, this->get_rbo_id());
}

[[nodiscard]] const GLuint& Renderbuffer::get_rbo_id() const noexcept {
  return this->_rbo_id;
}
} // namespace fre2d