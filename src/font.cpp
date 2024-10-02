#include <font.hpp>
#include <iostream>

namespace fre2d {
Font::Font(const FontManager &font_manager, const char *font_path,
           FT_UInt font_size) noexcept {
  this->initialize_font(font_manager, font_path, font_size);
}

Font::~Font() noexcept {
  if(this->_face) {
    FT_Done_Face(this->_face);
  }
}

void Font::initialize_font(const FontManager &font_manager, const char *font_path, FT_UInt font_size) {
  if(!fre2d::FontManager::is_initialized()) {
    font_manager.initialize();
  }
  this->_font_size = font_size;
  if(FT_New_Face(fre2d::FontManager::ft, font_path, 0, &this->_face) != 0) {
    std::cout << "error: Font::initialize(): failed to load font " << font_path << '\n';
    return;
  }
  FT_Set_Pixel_Sizes(this->_face, 0, this->_font_size);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // TODO: this only loads extended ASCII characters. use dynamic loader,
  // since we do not release FT_Face till Font destructor call, which means
  // we can still call FT_Load_Char. but we sure need to convert bytes to UTF-8
  // characters.
  for(unsigned char c = 0; c < 255; c++) {
    if(FT_Load_Char(this->_face, c, FT_LOAD_RENDER) != 0) {
      std::cout << "error: Font::initialize(): failed to load char '" << c << "'\n";
      continue;
    }
    Character ch;
    ch.texture.load_from_data(
      this->_face->glyph->bitmap.buffer,
      this->_face->glyph->bitmap.width,
      this->_face->glyph->bitmap.rows,
      false,
      false,
      Texture::WrapOptions {
        GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE
      },
      FormatRed
    );
    ch.size = glm::ivec2(this->_face->glyph->bitmap.width, this->_face->glyph->bitmap.rows);
    ch.bearing = glm::ivec2(this->_face->glyph->bitmap_left, this->_face->glyph->bitmap_top);
    ch.advance = this->_face->glyph->advance.x;
    this->_char_map.insert({c, ch});
  }
}
} // namespace fre2d