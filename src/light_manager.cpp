// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <light_manager.hpp>
#include <iostream>

namespace fre2d {
LightManager::LightManager() noexcept
  : _point_lights_diff{0}, _point_lights_modified{false}
{}

void LightManager::initialize() noexcept {
  // TODO: why not allocate more space during initialization?
  // like 16 slots preallocated and ready to be written to.
  this->_point_light_ssbo.empty_initialize(0);
}

void LightManager::update_buffers() noexcept {
  if(!this->_point_lights_modified)
    return;

  this->_point_lights_modified = false;

  if(this->_point_lights_diff != 0) {
    this->_point_light_ssbo.bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 sizeof(PointLight) * this->_point_lights.size(),
                 this->_point_lights.data(), GL_DYNAMIC_DRAW);
    this->_point_lights_diff = 0;
    return;
  }
  this->_point_light_ssbo.bind();
  // same buffer size, no new allocations.
  // maybe we can align the offset and size to fit only where parts have changed,
  // but idk if it will worth anything significant since 1 bulk data sent directly
  // might be better than multiple calls.
  glBufferSubData(
      GL_SHADER_STORAGE_BUFFER,
      0,
      sizeof(PointLight) * this->_point_lights.size(),
      this->_point_lights.data()
  );
}

[[nodiscard]] const std::vector<PointLight> &LightManager::get_point_lights() const noexcept {
  return this->_point_lights;
}

[[nodiscard]] const SSBO &LightManager::get_point_lights_ssbo() const noexcept {
  return this->_point_light_ssbo;
}

void LightManager::push_point_light(const PointLight &pl) noexcept {
  this->_point_lights.push_back(pl);
  this->_point_lights_modified = true;
  ++this->_point_lights_diff;
  this->update_buffers();
}

// index 1
// size 2
void LightManager::remove_point_light(std::size_t index) noexcept {
  this->check_size_and_index(index);
  this->_point_lights.erase(this->_point_lights.begin() + index);
  this->_point_lights_modified = true;
  --this->_point_lights_diff;
  this->update_buffers();
}

void LightManager::modify_point_light(std::size_t index, const PointLight &pl) noexcept {
  this->check_size_and_index(index);
  this->_point_lights[index] = pl;
  this->_point_lights_modified = true;
}

void LightManager::check_size_and_index(std::size_t index, const std::source_location &src) const {
  if(this->_point_lights.size() <= index) {
    std::cerr << "fre2d warning: LightManager::" << src.function_name() << "("
              << index
              << ") operation cannot be applied to buffer ID of "
              << this->_point_light_ssbo.get_ssbo_id()
              << ", current size of vector is "
              << this->_point_lights.size() << ".\n";
    std::abort();
  }
}

[[nodiscard]] const PointLight &LightManager::get_point_light(std::size_t index) const noexcept {
  this->check_size_and_index(index);
  return this->_point_lights[index];
}

[[nodiscard]] PointLight &LightManager::get_point_light_mutable(std::size_t index) noexcept {
  this->check_size_and_index(index);
  this->_point_lights_modified = true;
  return this->_point_lights[index];
}

[[nodiscard]] const AmbientLight& LightManager::get_ambient_light() const noexcept {
  return this->_ambient_light;
}

[[nodiscard]] AmbientLight& LightManager::get_ambient_light_mutable() noexcept {
  return this->_ambient_light;
}
} // namespace fre2d