// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <vector>
#include <source_location>
#include "light.hpp"
#include "ssbo.hpp"

namespace fre2d {
class LightManager {
public:
  LightManager() noexcept;

  void initialize() noexcept;
  void update_buffers() noexcept;

  [[nodiscard]] const std::vector<PointLight>& get_point_lights() const noexcept;
  [[nodiscard]] const SSBO& get_point_lights_ssbo() const noexcept;
  [[nodiscard]] const PointLight& get_point_light(std::size_t index) const noexcept;
  [[nodiscard]] PointLight& get_point_light_mutable(std::size_t index) noexcept;

  void push_point_light(const PointLight& pl) noexcept;
  void remove_point_light(std::size_t index) noexcept;
  void modify_point_light(std::size_t index, const PointLight& pl) noexcept;

  [[nodiscard]] const AmbientLight& get_ambient_light() const noexcept;

  [[nodiscard]] AmbientLight& get_ambient_light_mutable() noexcept;
protected:
  std::vector<PointLight> _point_lights;
  SSBO _point_light_ssbo;
  std::size_t _point_lights_diff;
  bool _point_lights_modified;

  AmbientLight _ambient_light;
private:
  void check_size_and_index(std::size_t index, const std::source_location& src = std::source_location::current()) const;
};
} // namespace fre2d