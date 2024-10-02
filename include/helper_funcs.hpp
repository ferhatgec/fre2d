// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <concepts>
#include <glm/glm.hpp>

namespace fre2d::detail {
template<std::floating_point Float>
[[nodiscard]]
static constexpr bool nearly_equals(const Float& a, const Float& b, const Float& epsilon = std::numeric_limits<Float>::epsilon()) noexcept {
  return a + epsilon >= b && a - epsilon <= b;
}

[[nodiscard]]
static constexpr bool nearly_equals(const glm::vec3& a, const glm::vec3& b) noexcept {
  return nearly_equals(a.x, b.x) && nearly_equals(a.y, b.y) && nearly_equals(a.z, b.z);
}

[[nodiscard]]
static constexpr bool nearly_equals(const glm::vec2& a, const glm::vec2& b) noexcept {
  return nearly_equals(a.x, b.x) && nearly_equals(a.y, b.y);
}
} // namespace fre2d