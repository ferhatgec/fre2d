// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace fre2d {
namespace detail::camera {
// practically you don't use them.
static constexpr GLfloat default_width { 800.f };
static constexpr GLfloat default_height { 600.f };
static constexpr glm::vec3 rotation_axis { 0.f, 0.f, 1.f };
static constexpr glm::vec3 empty_vec3 { 0.f, 0.f, 1.f }; // used for compare position vector
static constexpr glm::vec3 default_position { 0.f, 0.f, 1.f };
static constexpr glm::vec3 front_vec { -rotation_axis };
static constexpr glm::vec3 up_vec { 0.f, 1.f, 1.f };
} // namespace fre2d::detail::camera

class Camera {
public:
  // always keep width and height same as your framebuffer.
  explicit Camera(GLfloat width = detail::camera::default_width,
                  GLfloat height = detail::camera::default_height) noexcept;
  ~Camera() = default;

  void resize(GLfloat width, GLfloat height) noexcept;
  void set_position(const glm::vec3& position) noexcept; // change camera position to given position.
  void move_camera(const glm::vec3& position) noexcept; // move camera with given position
  void set_zoom_factor(GLfloat zoom_factor) noexcept;
  void rotate_camera(GLfloat radians) noexcept;

  [[nodiscard]] const glm::mat4& get_projection_matrix() const noexcept;
  [[nodiscard]] glm::mat4 get_view_matrix() const noexcept;
  [[nodiscard]] glm::mat4 get_view_matrix_no_zoom() const noexcept;
  [[nodiscard]] const glm::vec3& get_camera_position() const noexcept;
  [[nodiscard]] const GLfloat& get_zoom_factor() const noexcept;

  // don't update yourself since it won't make any difference, fre2d will do
  // it when it needs to be.
  void update_projection_matrix() noexcept;
private:
  glm::mat4 _projection;
  glm::mat4 _view, _view_nz;
  glm::vec3 _position;
  GLfloat _zoom_factor;
  GLfloat _width;
  GLfloat _height;
  GLfloat _rotation; // radians
};
} // namespace fre2d