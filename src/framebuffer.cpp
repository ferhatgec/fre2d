// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <framebuffer.hpp>
#include <iostream>
#include <renderer.hpp>

namespace fre2d {
Framebuffer::Framebuffer()
  : _first_time{true},
    _width{detail::renderer::default_width},
    _height{detail::renderer::default_height}, _clear_called{false} {}

Framebuffer::Framebuffer(GLsizei width,
                         GLsizei height,
                         bool use_default,
                         const char* default_vertex_shader,
                         const char* default_fragment_shader) noexcept
  : _first_time{true}, _clear_called{false} {
  this->initialize(width, height, use_default, default_vertex_shader, default_fragment_shader);
}

Framebuffer::~Framebuffer() noexcept {
  if(this->_fbo_id != 0) {
    glDeleteFramebuffers(1, &this->_fbo_id);
  }
}

void Framebuffer::bind() noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, this->get_fbo_id());
  // since fre2d::Framebuffer can work with custom or default framebuffers;
  // it's okay to use resize function; check for width and height deltas;
  // then do appropriated calls. like for default framebuffers just directly
  // calling glViewport; for customs rebuild depth & stencil renderbuffers and textures etc.
  glViewport(0, 0, this->get_width(), this->get_height());
  // TODO: z = 0 every time, so we don't need depth test at all. but it might be optional?
  glEnable(GL_DEPTH_TEST | GL_STENCIL_TEST);
}

void Framebuffer::initialize(GLsizei width,
                             GLsizei height,
                             bool use_default,
                             const char* default_vertex_shader,
                             const char* default_fragment_shader) noexcept {
  if(!this->_first_time) {
    std::cout << "error: cannot initialize same buffer more than once.\n";
    return;
  }
  this->_width = width;
  this->_height = height;
  if(use_default) {
    this->_fbo_id = 0; // default framebuffer
    // sync width and height for viewport.
    this->bind();
    this->unbind();
    this->_first_time = false;
    return;
  }
  // in resize(); we call initialize() so recompiling and -linking
  // shaders are not necessary.
  if(this->_shader.get_program_id() == 0) {
    this->_shader.initialize(detail::framebuffer::default_vertex, detail::framebuffer::default_fragment);
    this->_shader.use();
    this->_shader.set_int("ScreenTexture", 0);
  }

  // create framebuffer
  glGenFramebuffers(1, &this->_fbo_id);
  glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo_id);

  this->_color_buffer.load_nothing(width, height, false, false);
  this->_color_buffer.attach(*this);

  this->_depth_and_stencil_rb.initialize(width, height);
  this->_depth_and_stencil_rb.attach(*this);

  // TODO: remove shader defaults.
  this->_fb_vao.initialize();
  this->_fb_vbo.initialize(detail::framebuffer::framebuffer_vertices);
  this->_fb_vao.bind();
  this->_fb_vbo.bind();

  // position attribute (x, y)
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute (r, g, b, a)
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coordinate attribute (x, y)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  this->_first_time = false;
}

void Framebuffer::unbind() noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  if(this->get_fbo_id() != 0) {
    glDisable(GL_DEPTH_TEST | GL_STENCIL_TEST);
  }
}

// use at the end of render loop right before of swapping buffers.
void Framebuffer::render_texture() noexcept {
  if(this->get_fbo_id() != 0) {
    this->_shader.use();
    this->_fb_vao.bind();
    this->_fb_vbo.bind();
    this->_color_buffer.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->_fb_vao.unbind();
  } else {
    // if custom framebuffer is being used; then _clear_called won't make any difference
    // so we don't need to set it to false.
    this->_clear_called = false;
  }
}

void Framebuffer::resize(GLsizei width, GLsizei height) noexcept {
  // check if size changed
  if(width - this->get_width() != 0 || height - this->get_height() != 0) {
    this->_width = width;
    this->_height = height;

    if(this->get_fbo_id() != 0) {
      // there might be better way to do this;
      // but for now we recreate everything.
      glDeleteFramebuffers(1, &this->_fbo_id);
      this->_color_buffer = {};
      this->_depth_and_stencil_rb = {};
      this->_first_time = true;
      this->initialize(width, height);
      if(!this->is_complete()) {
        // TODO: use custom logging
        std::cout << "error: framebuffer is not complete\n";
      }
    }
    // update viewport
    glBindFramebuffer(GL_FRAMEBUFFER, this->get_fbo_id());
    glViewport(0, 0, this->get_width(), this->get_height());
    this->unbind();
  }
}

[[nodiscard]] const GLuint &Framebuffer::get_fbo_id() const noexcept {
  return this->_fbo_id;
}

[[nodiscard]] bool Framebuffer::is_complete() const noexcept {
  return glCheckNamedFramebufferStatus(this->get_fbo_id(), GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

[[nodiscard]] const GLsizei& Framebuffer::get_width() const noexcept {
  return this->_width;
}

[[nodiscard]] const GLsizei &Framebuffer::get_height() const noexcept {
  return this->_height;
}

  // notice: using this function with default framebuffer will return 0.
[[nodiscard]] const Texture& Framebuffer::get_color_buffer() const noexcept {
  return this->_color_buffer;
}

// notice: using this function with default framebuffer will return 0.
[[nodiscard]] const Renderbuffer& Framebuffer::get_depth_and_stencil_renderbuffer() const noexcept {
  return this->_depth_and_stencil_rb;
}

[[nodiscard]] const VertexArray& Framebuffer::get_framebuffer_vao() const noexcept {
  return this->_fb_vao;
}

[[nodiscard]] const VertexBuffer& Framebuffer::get_framebuffer_vbo() const noexcept {
  return this->_fb_vbo;
}

[[nodiscard]] const Shader& Framebuffer::get_framebuffer_shader() const noexcept {
  return this->_shader;
}

void Framebuffer::clear_color(GLfloat red, GLfloat green, GLfloat blue,
                              GLfloat alpha) noexcept {
  if(this->get_fbo_id() != 0) {
    Framebuffer::clear_color_force(red, green, blue, alpha);
    return;
  }

  // if framebuffer is default and _clear_called is false; then call for one time;
  // which will prevent custom framebuffer second clear_color call.
  if(!this->_clear_called) {
    Framebuffer::clear_color_force(red, green, blue, alpha);
    this->_clear_called = true;
  }
}

// difference between clear_color() and clear_color_force() is, where clear_color() checks
// to prevent clearing buffer if currently bound framebuffer is default one; clear_color_force()
// does not perform any checks about this; so you can use it directly.
// best way is do not use Framebuffer class unless you want to use custom framebuffer since
// you can't manipulate default framebuffer iirc.
void Framebuffer::clear_color_force(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) noexcept {
  glClearColor(red, green, blue, alpha);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
} // namespace fre2d