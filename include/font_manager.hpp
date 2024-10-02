// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <type_traits>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace fre2d {
namespace detail::font_manager {
static constexpr bool initialize_now { true };
} // namespace fre2d::detail::font_manager

// FT_Library is opaque pointer; but I kinda like hinting such that it's a pointer.
using FreeType_Library = std::remove_pointer_t<FT_Library>;
class FontManager {
public:
  static FreeType_Library* ft;

  explicit FontManager(bool initialize_now = detail::font_manager::initialize_now) noexcept;
  ~FontManager() noexcept;
  void initialize() const noexcept;

  [[nodiscard]] static bool is_initialized() noexcept;
};
} // namespace fre2d