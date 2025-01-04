// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>
#include <memory>

namespace fre2d {
struct WrapOptions;
class Texture;

namespace detail::texture {
static constexpr bool default_use_nearest { true };
static constexpr bool default_use_mipmap { true };
static constexpr GLuint default_wrap_x_opt { GL_CLAMP_TO_EDGE };
static constexpr GLuint default_wrap_y_opt { GL_CLAMP_TO_EDGE };
static constexpr GLuint default_internal_format { GL_RGBA8 };
static constexpr GLuint default_color_attachment { GL_COLOR_ATTACHMENT0 };
static constexpr unsigned char default_transparent_pixel[4] { 0, 0, 0, 0 };
} // namespace fre2d::detail::texture

enum ChannelsSpecifier : GLint {
  FormatBlue = -3,
  FormatGreen,
  FormatRed,
  FormatRgb = 3,
  FormatRgba
};

// TODO: support multisample textures
// also support texture units. we use GL_TEXTURE0 every time.
class Texture {
public:
  // for _framebuffer_load private function
  friend class Framebuffer;
  friend class Font;

  // sets GL_TEXTURE_WRAP_S and GL_TEXTURE_WRAP_T.
  // GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER is defined by use_nearest and use_mipmap.
  struct WrapOptions {
    constexpr explicit WrapOptions(GLuint wrap_x = detail::texture::default_wrap_x_opt,
                                   GLuint wrap_y = detail::texture::default_wrap_y_opt)
      : wrap_x_opt{wrap_x}, wrap_y_opt{wrap_y}
    {}

    [[nodiscard]]
    static constexpr WrapOptions default_value() noexcept {
      return WrapOptions();
    }

    GLuint wrap_x_opt; // x = s = u
    GLuint wrap_y_opt; // y = t = v
  };

  // initialize it later using load() or _framebuffer_load if you are Framebuffer :)
  Texture() noexcept;
  Texture(GLuint texture_id) noexcept;

  explicit Texture(
    const char* file_path,
    GLsizei width = -1,
    GLsizei height = -1,
    bool use_nearest = detail::texture::default_use_nearest,
    bool use_mipmap = detail::texture::default_use_mipmap,
    const WrapOptions& texture_wrap = WrapOptions::default_value()
  ) noexcept;

  ~Texture();

  void load(GLuint texture_id) noexcept;
  void load_override(GLuint texture_id) noexcept;
  void release() noexcept;

  // set use_nearest = true for pixelated, low-res images.
  // set use_nearest = false to achieve smooth transitions.
  void load(
    const char* file_path,
    GLsizei width = -1,
    GLsizei height = -1,
    bool use_nearest = detail::texture::default_use_nearest,
    bool use_mipmap = detail::texture::default_use_mipmap,
    const WrapOptions& texture_wrap = WrapOptions::default_value()
  ) noexcept;

  void load_from_data(
    const unsigned char* image_data,
    GLsizei width = 1,
    GLsizei height = 1,
    bool use_nearest = detail::texture::default_use_nearest,
    bool use_mipmap = detail::texture::default_use_mipmap,
    const WrapOptions& texture_wrap = WrapOptions::default_value(),
    int channels = 4
  ) noexcept;

  // no checks, just calls what you need
  // set use_mipmap as false, for framebuffer color buffer.
  // we set use_nearest as false for it too.
  void load_nothing(
    GLsizei width = 1,
    GLsizei height = 1,
    bool use_nearest = detail::texture::default_use_nearest,
    bool use_mipmap = detail::texture::default_use_mipmap,
    const WrapOptions& texture_wrap = WrapOptions::default_value(),
    int channels = 4
  ) noexcept;

  void set_parameters(
    bool use_nearest = detail::texture::default_use_nearest,
    bool use_mipmap = detail::texture::default_use_mipmap,
    const WrapOptions& texture_wrap = WrapOptions::default_value()
  ) noexcept;

  [[nodiscard]] static const Texture& get_default_texture() noexcept;

  void bind(GLuint texture_unit = 0) const noexcept;
  void unbind() const noexcept;
  void attach(const Framebuffer& fb, GLuint attachment = detail::texture::default_color_attachment) const noexcept;

  [[nodiscard]] const GLuint& get_texture_id() const noexcept;

  friend bool operator==(const Texture& lhs, const Texture& rhs) noexcept {
    return lhs.get_texture_id() == rhs.get_texture_id();
  }

  friend bool operator!=(const Texture& lhs, const Texture& rhs) noexcept {
    return !operator==(lhs, rhs);
  }
private:
  friend class Framebuffer;
  GLint _internal_format;
  GLint _format;
  // we need to track it since it can be copied, which means; destructor will be
  // invoked more than once. maybe it should be better if we just use the same approach
  // as the below one; otherwise we need to wrap _internal_format and _format as pointer too;
  // that's because copy would make them differ from others, so one would be GL_RGB;
  // and otherwise GL_RGBA? so it's TODO:
  // https://www.khronos.org/opengl/wiki_opengl/index.php?title=Common_Mistakes#RAII_and_hidden_destructor_calls
  std::shared_ptr<GLuint> _texture_id;
};
} // namespace fre2d