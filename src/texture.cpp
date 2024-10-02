// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <texture.hpp>
#include <framebuffer.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace fre2d {
Texture::Texture() noexcept
    : _internal_format{detail::texture::default_internal_format} {
  this->_texture_id = std::make_shared<GLuint>(0);
}

Texture::Texture(GLuint texture_id) noexcept : _texture_id{std::make_shared<GLuint>(texture_id)} {}

Texture::Texture(const char* file_path,
                 GLsizei width,
                 GLsizei height,
                 bool use_nearest,
                 bool use_mipmap,
                 const WrapOptions& texture_wrap) noexcept
  : _internal_format{detail::texture::default_internal_format} {
  this->_texture_id = std::make_shared<GLuint>(0);
  this->load(file_path, width, height, use_nearest, use_mipmap, texture_wrap);
}

Texture::~Texture() {
  this->release();
}

/// change _texture_id to texture_id with releasing current texture.
void Texture::load(GLuint texture_id) noexcept {
  this->release();
  this->_texture_id = std::make_shared<GLuint>(texture_id);
}

/// change _texture_id to texture_id without releasing current texture.
/// but you need to get_texture_id() before using this to manage/release it yourself.
void Texture::load_override(GLuint texture_id) noexcept {
  this->_texture_id = std::make_shared<GLuint>(texture_id);
}

/// remove current Texture and set _texture_id to 0.
/// if _texture_id is already 0, then no action will be performed.
void Texture::release() noexcept {
  // we check for use_count because there might be multiple copies of this Texture;
  // and when one of them goes out of scope, destructor will be called more than once,
  // and then glDeleteTextures will set _texture_id to 0;
  // other copies still would think _texture_id is have not still been released.
  // but their _texture_id will probably be invalid.
  if(this->get_texture_id() != 0 && this->_texture_id.use_count() == 1) {
    glDeleteTextures(1, &*this->_texture_id);
    *this->_texture_id = 0;
  }
}

void Texture::load(const char* file_path,
                   GLsizei width,
                   GLsizei height,
                   bool use_nearest,
                   bool use_mipmap,
                   const WrapOptions& texture_wrap) noexcept {
  // width & height is -1 by default, if file_path is not nullptr,
  // then stb will automatically detect width and height;
  // but we added them as optional, so can be used to define them explicitly
  // as we use Texture class in Framebuffer.
  // so if you use width and height; then pass file_path = nullptr;
  // otherwise it'll be overwritten by stb; it's mostly for Framebuffer
  // to reduce use of OpenGL outside renderer.
  int w { width != -1 ? width : 0 };
  int h { height != -1 ? height : 0 };
  int channels { 4 }; // since it's possible to pass file_path = nullptr, we initialize them first.

  stbi_set_flip_vertically_on_load(true);
  auto* image_data = file_path ? stbi_load(file_path, &w, &h, &channels, 0) : nullptr;

  // file is specified but image data is nullptr.
  if(!image_data && file_path) {
    // TODO: use custom logging module,
    // in render loop (which provided by user),
    // add function to check if fre2d have to be closed; encountered any errors; like this
    // while(!glfwWindowShouldClose(window_ptr) && fre2d_is_running())
    // for errors; we will set explicitly fre2d_is_running bool instance value to false.
    std::cout << "error: cannot load image file " << file_path << '\n';
  }
  this->load_from_data(image_data, w, h, use_nearest, use_mipmap, texture_wrap, channels);
  if(image_data) {
    stbi_image_free(image_data);
  }
}

void Texture::load_from_data(const unsigned char *image_data, GLsizei width, GLsizei height, bool use_nearest,
                             bool use_mipmap, const WrapOptions& texture_wrap,
                             int channels) noexcept {
  // we always store them as GL_RGBA in internal format; format may variate.
  // precision is enough for 2D games but
  // GL_RGB/GL_RGBA8 might be an option.
  // TODO: add an option to set internal format.

  // exclusive cases;
  // * use channels = -1 to set both _format and _internal_format to GL_RED
  // * use channels = -2 to set both _format and _internal_format to GL_GREEN
  // * use channels = -3 to set both _format and _internal_format to GL_BLUE
  // otherwise, use 3 for GL_RGB; 4 for GL_RGBA; but _internal_format is always
  // GL_RGBA.
  switch(channels) {
  case FormatRed: { this->_format = this->_internal_format = GL_RED; break; }
  case FormatGreen: { this->_format = this->_internal_format = GL_GREEN; break; }
  case FormatBlue: { this->_format = this->_internal_format = GL_BLUE; break; }
  default: { this->_format = channels == 4 ? GL_RGBA : GL_RGB; break; }
  }
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &*this->_texture_id);
  glBindTexture(GL_TEXTURE_2D, *this->_texture_id);
  // special case for FormatRed, FormatGreen and FormatBlue, mostly
  // used for Font class.
  if(this->_format == GL_RED ||
     this->_format == GL_GREEN ||
     this->_format == GL_BLUE) {
    // mutable; unlike glTextureStorage2D.
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      this->_internal_format,
      width,
      height,
      0,
      this->_format,
      GL_UNSIGNED_BYTE,
      image_data
    );
  } else {
    // generate immutable storage for general purpose.
    glTextureStorage2D(
      this->get_texture_id(),
      1,
      this->_internal_format,
      width,
      height
    );
    // since we are dealing with C API,
    // passing NULL is probably more comprehensible than nullptr.
    // file_path = nullptr -> _framebuffer_load(); if you want to pass raw pixels;
    // it's not available yet but not hard to implement too. (would be great and
    // easy TODO) then we can call it with parameters image_data, internal_format,
    // width, height, format etc.
    glTextureSubImage2D(
      this->get_texture_id(),
      0,
      0,
      0,
      width,
      height,
      this->_format,
      GL_UNSIGNED_BYTE,
      image_data ? image_data : NULL
    );

    // 1x1 is special case since we use it as default sampler2D;
    // also, 1x1s has nothing to do with mipmaps right?
    if (use_mipmap && width != 1 && height != 1) {
      glGenerateTextureMipmap(this->get_texture_id());
    }
  }
  this->set_parameters(use_nearest, use_mipmap, texture_wrap);
}
void Texture::load_nothing(GLsizei width, GLsizei height, bool use_nearest, bool use_mipmap,
                           const WrapOptions& texture_wrap, int channels) noexcept {
  this->_format = channels == 4 ? GL_RGBA : GL_RGB;
  glGenTextures(1, &*this->_texture_id);
  glBindTexture(GL_TEXTURE_2D, *this->_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, this->_internal_format, width, height, 0, this->_format, GL_UNSIGNED_BYTE, NULL);
  this->set_parameters(use_nearest, use_mipmap, texture_wrap);
}

void Texture::set_parameters(bool use_nearest, bool use_mipmap, const WrapOptions& texture_wrap) noexcept {
  glTextureParameteri(this->get_texture_id(), GL_TEXTURE_WRAP_S, texture_wrap.wrap_x_opt);
  glTextureParameteri(this->get_texture_id(), GL_TEXTURE_WRAP_T, texture_wrap.wrap_y_opt);
  if(use_nearest) {
    glTextureParameteri(this->get_texture_id(), GL_TEXTURE_MIN_FILTER,
                        use_mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
    glTextureParameteri(this->get_texture_id(), GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
  } else {
    glTextureParameteri(this->get_texture_id(), GL_TEXTURE_MIN_FILTER,
                        use_mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(this->get_texture_id(), GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);
  }
}

// generates 1x1 transparent texture that used as default uniform texture to
// prevent UB.
[[nodiscard]] const Texture& Texture::get_default_texture() noexcept {
  static Texture default_texture;
  if(default_texture.get_texture_id() == 0) {
    default_texture.load_from_data(detail::texture::default_transparent_pixel);
  }
  return default_texture;
}

void Texture::bind(GLuint texture_unit) const noexcept {
  // TODO: check for maximum texture units
  glBindTextureUnit(texture_unit, this->get_texture_id());
}

void Texture::unbind() const noexcept {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::attach(const Framebuffer& fb, GLuint attachment) const noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, fb.get_fbo_id());
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *this->_texture_id, 0);
}

[[nodiscard]] const GLuint& Texture::get_texture_id() const noexcept {
  return *this->_texture_id;
}
} // namespace fre2d