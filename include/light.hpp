// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glm/glm.hpp>

namespace fre2d {
namespace detail::ambient_light {
static constexpr auto default_color = glm::vec4 { 1.f, 1.f, 1.f, 1.f };
} // namespace fre2d::detail::ambient_light

namespace detail::point_light {
// just dummy values (for fresh), you should set it at your own for better results.
static constexpr auto default_ambient = glm::vec3 { 1.f, 1.f, 1.f };
static constexpr auto default_diffuse = glm::vec3 { 0.f, 0.f, 0.f };
static constexpr auto default_att_constant = 0.f;
static constexpr auto default_att_linear = 0.009f;
static constexpr auto default_att_quadratic = 0.00032f;
static constexpr auto default_disabled = false;
} // namespace fre2d::detail::point_light

// TODO: implement AmbientLight and DirectionalLight.
class PointLight {
public:
  PointLight(
    glm::vec2 position,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    float constant,
    float linear,
    float quadratic,
    bool disabled = detail::point_light::default_disabled
  ) : _position{position},
      _ambient{glm::vec4(ambient, 0.f)},
      _diffuse{glm::vec4(diffuse, 0.f)},
      _constant{constant},
      _linear{linear},
      _quadratic{quadratic},
      _disabled(disabled)
  {}

  void set_position(const glm::vec2& position) noexcept;
  void set_ambient(const glm::vec3& ambient) noexcept;
  void set_diffuse(const glm::vec3& diffuse) noexcept;
  void set_attenuation_constant(float constant) noexcept;
  void set_attenuation_linear(float linear) noexcept;
  void set_attenuation_quadratic(float quadratic) noexcept;
  void set_disabled(bool disabled) noexcept;

  [[nodiscard]] const glm::vec2& get_position() const noexcept;
  [[nodiscard]] const glm::vec4& get_ambient() const noexcept;
  [[nodiscard]] const glm::vec4& get_diffuse() const noexcept;
  [[nodiscard]] const float& get_attenuation_constant() const noexcept;
  [[nodiscard]] const float& get_attenuation_linear() const noexcept;
  [[nodiscard]] const float& get_attenuation_quadratic() const noexcept;
  [[nodiscard]] const int& get_disabled() const noexcept;

  [[nodiscard]] glm::vec2& get_position_mutable() noexcept;
  [[nodiscard]] glm::vec4& get_ambient_mutable() noexcept;
  [[nodiscard]] glm::vec4& get_diffuse_mutable() noexcept;
  [[nodiscard]] float& get_attenuation_constant_mutable() noexcept;
  [[nodiscard]] float& get_attenuation_linear_mutable() noexcept;
  [[nodiscard]] float& get_attenuation_quadratic_mutable() noexcept;
  [[nodiscard]] int& get_disabled_mutable() noexcept;
private:
  glm::vec2 _position;
  float _pad0[2]; // required for std430 representation

  glm::vec4 _ambient;
  glm::vec4 _diffuse;

  float _constant;
  float _linear;
  float _quadratic;

  int _disabled; // required for std430 representation
};

class AmbientLight {
public:
  AmbientLight(glm::vec4 color = detail::ambient_light::default_color)
      : _color{color}
  {}

  void set_color(const glm::vec4& color) noexcept;
  [[nodiscard]] const glm::vec4& get_color() const noexcept;
  [[nodiscard]] glm::vec4& get_color_mutable() noexcept;
private:
  glm::vec4 _color;
};
} // namespace fre2d