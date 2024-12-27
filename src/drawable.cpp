// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <glm/gtc/matrix_transform.hpp>
#include <drawable.hpp>
#include <renderbuffer.hpp>
#include <helper_funcs.hpp>
#include <camera.hpp>

namespace fre2d {
Drawable::Drawable() noexcept
  : _position{detail::drawable::default_position},
    _scale{detail::drawable::default_scale},
    _rotation_rads{detail::drawable::default_rotation_radians},
    _model_matrix_update_required{true},
    _flip_vertically{detail::drawable::default_flip_vertically},
    _flip_horizontally{detail::drawable::default_flip_horizontally},
    _relative_pos{0.f, 0.f} {
}

Drawable::Drawable(
  const glm::vec3& scale,
  const glm::vec2& position,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept
  : _relative_pos{0.f, 0.f} {
  this->initialize_drawable(
    scale,
    position,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Drawable::set_position(const glm::vec2& position) noexcept {
  if(detail::nearly_equals(this->_position, position)) {
    return;
  }
  this->_position = position;
  this->_model_matrix_update_required = true;
}

void Drawable::set_rotation(GLfloat rotation_rads) noexcept {
  if(detail::nearly_equals(this->_rotation_rads, rotation_rads)) {
    return;
  }
  this->_rotation_rads = rotation_rads;
  this->_model_matrix_update_required = true;
}

void Drawable::set_scale(const glm::vec3& scale) noexcept {
  if(detail::nearly_equals(detail::drawable::default_scale, scale)) {
    return;
  }
  this->_scale = scale;
  this->_model_matrix_update_required = true;
}

[[nodiscard]] const glm::vec2& Drawable::get_position() const noexcept {
  return this->_position;
}

[[nodiscard]] const glm::vec3& Drawable::get_scale() const noexcept {
  return this->_scale;
}

[[nodiscard]] const GLfloat& Drawable::get_rotation() const noexcept {
  return this->_rotation_rads;
}

// TODO: mesh might be uninitialized. so we need to check.
[[nodiscard]] const Mesh& Drawable::get_mesh() const noexcept {
  return this->_mesh;
}

[[nodiscard]] const glm::mat4 &Drawable::get_model_matrix() noexcept {
  if (this->is_matrix_update_required()) {
    // actually it's for Label so we may separate it later.
    const glm::vec2 real_pos = this->_position + this->_relative_pos;
    this->_model =
        glm::translate(glm::mat4(1.f), glm::vec3(real_pos - glm::vec2{this->_relative_pos.x, 0.f}, 0.f));
    this->_model = glm::rotate(this->_model, this->_rotation_rads,
                               detail::camera::rotation_axis);
    if(!detail::nearly_equals(this->_relative_pos, {0.f, 0.f})) {
      this->_model =
          glm::translate(this->_model, glm::vec3(-real_pos, 0.f));
    }
    this->_model = glm::scale(this->_model, this->_scale);
    this->_model_matrix_update_required = false;
  }
  return this->_model;
}

[[nodiscard]] glm::mat4
Drawable::get_model_matrix_custom(const glm::vec3 &scale,
                                  const glm::vec2 &position,
                                  GLfloat rotation_rads) noexcept {
  glm::mat4
  model = glm::translate(glm::mat4(1.f), glm::vec3(position, 0.f));
  model = glm::rotate(model, rotation_rads, detail::camera::rotation_axis);
  model = glm::scale(model, scale);
  return model;
}

[[nodiscard]] const bool& Drawable::is_matrix_update_required() const noexcept {
  return this->_model_matrix_update_required;
}

void Drawable::initialize_drawable(
  const glm::vec3& scale,
  const glm::vec2& position,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->_position = position;
  this->_rotation_rads = rotation_rads;
  this->_scale = scale;
  this->_model_matrix_update_required = true;
  this->_flip_vertically = flip_vertically;
  this->_flip_horizontally = flip_horizontally;
}

// sets shader uniforms
void Drawable::before_draw(const Shader& shader,
                           const std::unique_ptr<Camera>& camera) noexcept {
  this->get_mesh().get_vao().bind();
  shader.use();
  shader.set_float_mat4x4("Model", this->get_model_matrix());
  shader.set_float_mat4x4("View", camera->get_view_matrix());
  shader.set_float_mat4x4("Projection", camera->get_projection_matrix());
  shader.set_bool("UseTexture", this->get_mesh().get_texture().has_value());
  shader.set_bool("FlipVertically", this->_flip_vertically);
  shader.set_bool("FlipHorizontally", this->_flip_horizontally);
  this->before_draw_custom(shader, camera);
  shader.set_int("TextureSampler", 0);
  // no texture given
  if (!this->get_mesh().get_texture().has_value()) {
    Texture::get_default_texture().bind(0);
  } else {
    this->get_mesh().get_texture()->bind(0);
  }
  this->get_mesh().get_vao().unbind();
}

void Drawable::before_draw_custom(const Shader& shader, const std::unique_ptr<Camera>& camera) noexcept {}
} // namespace fre2d