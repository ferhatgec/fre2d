// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <memory>
#include "camera.hpp"
#include "framebuffer.hpp"


// fresh renderer enhanced 2d
// (OpenGL based renderer for freshengine but can be used for other things as well)
// (that's why this is in separate repo)
namespace fre2d {
namespace detail::renderer {
static constexpr GLsizei default_width { 800 };
static constexpr GLsizei default_height { 600 };
static constexpr auto default_tests { GL_DEPTH_TEST | GL_STENCIL_TEST };

} // namespace fre2d::detail::renderer

class Renderer {
public:
  Renderer() noexcept;
  Renderer(GLsizei width, GLsizei height) noexcept;
  ~Renderer() = default;

  void attach_framebuffer(std::unique_ptr<Framebuffer> fb) noexcept;
  void attach_camera(std::unique_ptr<Camera> cam) noexcept;

  // update camera and framebuffer size
  void resize(GLsizei width, GLsizei height) noexcept;

  [[nodiscard]] const std::unique_ptr<Framebuffer>& get_framebuffer() const noexcept;
  [[nodiscard]] const std::unique_ptr<Camera>& get_camera() const noexcept;
  [[nodiscard]] const GLsizei& get_width() const noexcept;
  [[nodiscard]] const GLsizei& get_height() const noexcept;

  [[nodiscard]] const bool& is_initialized() const noexcept;
private:
  std::unique_ptr<Framebuffer> _framebuffer;
  std::unique_ptr<Camera> _camera;
  GLsizei _width;
  GLsizei _height;
  bool _initialized;
};
} // namespace fre2d