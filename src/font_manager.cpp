// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <font_manager.hpp>
#include <iostream>

namespace fre2d {
FreeType_Library* FontManager::ft = nullptr;

FontManager::FontManager(bool initialize_now) noexcept {
  if(initialize_now) {
    this->initialize();
  }
}

FontManager::~FontManager() noexcept {
  if(FontManager::ft) {
    FT_Done_FreeType(FontManager::ft);
  }
}

void FontManager::initialize() const noexcept {
  if (FontManager::ft) {
    // TODO: implement simple logging function for this
    std::cout << "info: FontManager::initialize(): already initialized.\n";
    return;
  }
  if(FT_Init_FreeType(&FontManager::ft) != 0) {
    std::cout
        << "error: FontManager::initialize(): cannot initialize FreeType!\n";
    FontManager::ft = nullptr;
  }
}

[[nodiscard]] bool FontManager::is_initialized() noexcept {
  return FontManager::ft != nullptr;
}
} // namespace fre2d