// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <renderer.hpp>
#include <iostream>

namespace fre2d {
Renderer::Renderer() noexcept
  : _width{detail::renderer::default_width},
    _height{detail::renderer::default_height},
    _initialized{false} {}

Renderer::Renderer(GLsizei width, GLsizei height) noexcept
  : _width{width}, _height{height}, _initialized{false} {
  this->attach_framebuffer(std::make_unique<Framebuffer>(width, height));
  this->attach_camera(std::make_unique<Camera>(width, height));
  this->_initialized = true;
}

void Renderer::attach_framebuffer(std::unique_ptr<Framebuffer> fb) noexcept {
  this->_framebuffer = std::move(fb);
  if(this->_camera && !this->_initialized) {
    this->_initialized = true;
  }
}

void Renderer::attach_camera(std::unique_ptr<Camera> cam) noexcept {
  this->_camera = std::move(cam);
  if(this->_framebuffer && !this->_initialized) {
    this->_initialized = true;
  }
}

void Renderer::resize(GLsizei width, GLsizei height) noexcept {
  if(this->is_initialized()) {
    this->_framebuffer->resize(width, height);
    this->_camera->resize(width, height);
  } else {
    std::cout << "fre2d error: cannot resize renderer since either framebuffer or camera is not yet initialized.\n";
  }
}

[[nodiscard]] const std::unique_ptr<Framebuffer>& Renderer::get_framebuffer() const noexcept {
  return this->_framebuffer;
}

[[nodiscard]] const std::unique_ptr<Camera>& Renderer::get_camera() const noexcept {
  return this->_camera;
}

[[nodiscard]] const GLsizei& Renderer::get_width() const noexcept {
  return this->_width;
}

[[nodiscard]] const GLsizei& Renderer::get_height() const noexcept {
  return this->_height;
}

[[nodiscard]] const bool& Renderer::is_initialized() const noexcept {
  return this->_initialized;
}
} // namespace fre2d