// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "font_manager.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include <unordered_map>

namespace fre2d {
namespace detail::font {
static constexpr FT_UInt default_font_height { 36 };
} // namespace fre2d::detail::font

using FreeType_Face = std::remove_pointer_t<FT_Face>;

class Font {
public:
  friend class Label;
  struct Character {
    Texture texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
  };

  Font() noexcept = default;
  Font(
    const FontManager& font_manager,
    const char* font_path,
    FT_UInt font_size = detail::font::default_font_height
  ) noexcept;
  ~Font() noexcept;

  void initialize_font(
    const FontManager& font_manager,
    const char* font_path,
    FT_UInt font_size = detail::font::default_font_height
  );
private:
  FreeType_Face* _face;
  FT_UInt _font_size;
  std::unordered_map<char, Character> _char_map;
};
} // namespace fre2d