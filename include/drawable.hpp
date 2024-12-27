// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "mesh.hpp"
#include "shader.hpp"
#include <memory>

namespace fre2d {
namespace detail::drawable {
static constexpr glm::vec2 default_position { 0.f, 0.f };
static constexpr GLfloat default_rotation_radians { 0.f };
static constexpr glm::vec3 default_scale { 1.f, 1.f, 1.f };
static constexpr glm::vec4 default_color { 1.f, 1.f, 1.f, 1.f };
static constexpr bool default_flip_vertically { false };
static constexpr bool default_flip_horizontally { false };
} // namespace fre2d::detail::drawable

class Camera;
class Drawable {
public:
  Drawable() noexcept;

  explicit Drawable(
    const glm::vec3& scale,
    const glm::vec2& position = detail::drawable::default_position,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_vertically
  ) noexcept;

  virtual ~Drawable() = default;

  /// rotate counter-clockwise winding
  void set_position(const glm::vec2& position) noexcept;
  void set_rotation(GLfloat rotation_rads) noexcept;
  void set_scale(const glm::vec3& scale) noexcept;

  [[nodiscard]] const glm::vec2& get_position() const noexcept;
  [[nodiscard]] const glm::vec3& get_scale() const noexcept;
  [[nodiscard]] const GLfloat& get_rotation() const noexcept;
  [[nodiscard]] const Mesh& get_mesh() const noexcept;
  [[nodiscard]] const glm::mat4& get_model_matrix() noexcept;
  [[nodiscard]] static glm::mat4 get_model_matrix_custom(
    const glm::vec3& scale,
    const glm::vec2& position = detail::drawable::default_position,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians
  ) noexcept;

  [[nodiscard]] const bool& is_matrix_update_required() const noexcept;

  void initialize_drawable(
    const glm::vec3& scale,
    const glm::vec2& position = detail::drawable::default_position,
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_vertically
  ) noexcept;

  virtual void draw(const Shader& shader, const std::unique_ptr<Camera>& camera) noexcept = 0;
  virtual void before_draw(const Shader& shader, const std::unique_ptr<Camera>& camera) noexcept; // set uniforms, including camera matrices
  virtual void before_draw_custom(const Shader& shader, const std::unique_ptr<Camera>& camera) noexcept;
protected:
  glm::vec2 _position;
  glm::vec3 _scale; // normally in 2D space you don't need z dimension but i added it anyway.
  GLfloat _rotation_rads;
  Mesh _mesh;
  glm::mat4 _model;
  glm::vec2 _relative_pos;
  bool _model_matrix_update_required;
  bool _flip_vertically, _flip_horizontally;
};
} // namespace fre2d