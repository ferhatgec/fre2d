// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <light.hpp>

namespace fre2d {
void PointLight::set_position(const glm::vec2 &position) noexcept {
  this->_position = position;
}

void PointLight::set_ambient(const glm::vec3 &ambient) noexcept {
  this->_ambient = glm::vec4(ambient, 0.f);
}

void PointLight::set_diffuse(const glm::vec3 &diffuse) noexcept {
  this->_diffuse = glm::vec4(diffuse, 0.f);
}

void PointLight::set_attenuation_constant(float constant) noexcept {
  this->_constant = constant;
}

void PointLight::set_attenuation_linear(float linear) noexcept {
  this->_linear = linear;
}

void PointLight::set_attenuation_quadratic(float quadratic) noexcept {
  this->_quadratic = quadratic;
}

[[nodiscard]] const glm::vec2 &PointLight::get_position() const noexcept {
  return this->_position;
}

[[nodiscard]] const glm::vec4 &PointLight::get_ambient() const noexcept {
  return this->_ambient;
}

[[nodiscard]] const glm::vec4 &PointLight::get_diffuse() const noexcept {
  return this->_diffuse;
}

[[nodiscard]] const float &PointLight::get_attenuation_constant() const noexcept {
  return this->_constant;
}

[[nodiscard]] const float &PointLight::get_attenuation_linear() const noexcept {
  return this->_linear;
}

[[nodiscard]] const float &PointLight::get_attenuation_quadratic() const noexcept {
  return this->_quadratic;
}

[[nodiscard]] glm::vec2 &PointLight::get_position_mutable() noexcept {
  return this->_position;
}

[[nodiscard]] glm::vec4 &PointLight::get_ambient_mutable() noexcept {
  return this->_ambient;
}

[[nodiscard]] glm::vec4 &PointLight::get_diffuse_mutable() noexcept {
  return this->_diffuse;
}

[[nodiscard]] float &PointLight::get_attenuation_constant_mutable() noexcept {
  return this->_constant;
}

[[nodiscard]] float &PointLight::get_attenuation_linear_mutable() noexcept {
  return this->_linear;
}

[[nodiscard]] float &PointLight::get_attenuation_quadratic_mutable() noexcept {
  return this->_quadratic;
}

void AmbientLight::set_color(const glm::vec4& color) noexcept {
  this->_color = color;
}

[[nodiscard]] const glm::vec4& AmbientLight::get_color() const noexcept {
  return this->_color;
}

[[nodiscard]] glm::vec4& AmbientLight::get_color_mutable() noexcept {
  return this->_color;
}
} // namespace fre2d