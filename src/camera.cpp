// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <camera.hpp>
#include <helper_funcs.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace fre2d {
Camera::Camera(GLfloat width, GLfloat height) noexcept
    : _position{0.f, 0.f, 1.f}, _zoom_factor{1.f}, _width{width},
      _height{height} {
  this->update_projection_matrix();
}

void Camera::resize(GLfloat width, GLfloat height) noexcept {
  // check if size is same. so we don't need to update projection matrix.
  if(detail::nearly_equals(this->_width, width) &&
     detail::nearly_equals(this->_height, height)) {
    return;
  }
  this->_width = width;
  this->_height = height;
  this->update_projection_matrix();
}

void Camera::set_position(const glm::vec3& position) noexcept {
  // check if position is same so we don't need to update view matrix.
  if(detail::nearly_equals(this->_position, position)) {
    return;
  }
  this->_position = position;
}

void Camera::move_camera(const glm::vec3& position) noexcept {
  // check if position nearly equal to (0, 0, 0) so we don't need to update view matrix.
  if(detail::nearly_equals(position, detail::camera::empty_vec3)) {
    return;
  }
  this->_position += position;
}

void Camera::set_zoom_factor(GLfloat zoom_factor) noexcept {
  if(detail::nearly_equals(this->_zoom_factor, zoom_factor)) {
    return;
  }
  this->_zoom_factor = std::fmaxf(0.f, zoom_factor);
}

void Camera::rotate_camera(GLfloat radians) noexcept {
  if(detail::nearly_equals(this->_rotation, radians)) {
    return;
  }
  this->_rotation = radians;
}

[[nodiscard]] const glm::mat4& Camera::get_projection_matrix() const noexcept {
  return this->_projection;
}

// TODO: why not trigger it to update view matrix only when some parts of it changed?
[[nodiscard]] glm::mat4 Camera::get_view_matrix() const noexcept {
  return glm::scale(glm::lookAt(
    this->_position,
    this->_position + detail::camera::front_vec,
    detail::camera::up_vec
  ), glm::vec3(this->_zoom_factor, this->_zoom_factor, 1.f));
}

[[nodiscard]] glm::mat4 Camera::get_view_matrix_no_zoom() const noexcept {
  return glm::scale(glm::lookAt(
    this->_position,
    this->_position + detail::camera::front_vec,
    detail::camera::up_vec
  ), glm::vec3(1.f, 1.f, 1.f));
}

[[nodiscard]] const glm::vec3& Camera::get_camera_position() const noexcept {
  return this->_position;
}

[[nodiscard]] const GLfloat& Camera::get_zoom_factor() const noexcept {
  return this->_zoom_factor;
}

// set (0, 0) as its center.
void Camera::update_projection_matrix() noexcept {
  this->_projection = glm::ortho(
    -this->_width / 2.f,
    this->_width / 2.f,
    -this->_height / 2.f,
    this->_height / 2.f,
    -1.f,
    1.f
  );
}
} // namespace fre2d