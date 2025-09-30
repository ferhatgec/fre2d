// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <camera.hpp>
#include <chrono>
#include <label.hpp>

#include "../include/helper_funcs.hpp"
#include <renderer.hpp>

namespace fre2d {
Label::Label(
  const Font &font,
  const char *text,
  const glm::vec2 &position,
  const glm::vec4 &color,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->initialize_label(
    font,
    text,
    position,
    color,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

Label::Label(
  const Font &font,
  const char *text,
  const glm::vec2 &position,
  const std::array<glm::vec4, 6> &colors,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->initialize_label(
    font,
    text,
    position,
    colors,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

Label::~Label() {}

void Label::initialize_label(
  const Font &font,
  const char *text,
  const glm::vec2 &position,
  const glm::vec4 &color,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->_colors = std::array<glm::vec4, 6> {color, color, color, color, color, color};
  this->_initialize_fields_other_than_color(
    font,
    text,
    position,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Label::initialize_label(
  const Font &font,
  const char *text,
  const glm::vec2 &position,
  const std::array<glm::vec4, 6> &colors,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->_colors = colors;
  this->_initialize_fields_other_than_color(
    font,
    text,
    position,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

// TODO: actually it's pretty straightforward to use Mesh class here.
// since we utilize both VertexArray and VertexBuffer.
void Label::draw(const Shader &shader,
                 const std::unique_ptr<Renderer> &rnd) noexcept {
  this->draw(shader, rnd->get_camera(), rnd->get_light_manager());
}

void Label::before_draw(const Shader &shader,
                        const std::unique_ptr<Renderer> &rnd) noexcept {
  this->before_draw(shader, rnd->get_camera(), rnd->get_light_manager());
}


void Label::draw(const Shader &shader, const std::unique_ptr<Camera> &cam,
                 const std::unique_ptr<LightManager> &lm) noexcept {
  glm::vec2 pos = this->_position;
  this->before_draw(shader, cam, lm);
  glActiveTexture(GL_TEXTURE0);
  for (const auto &c : this->_text) {
    const auto &character = this->_font._char_map[c];
    float xpos = pos.x + static_cast<float>(character.bearing.x);
    float ypos = pos.y - static_cast<float>(character.size.y - character.bearing.y);
    const auto w = static_cast<float>(character.size.x);
    const auto h = static_cast<float>(character.size.y);
    // use one color for every vertex.
    std::array<Vertex, 6> vertices;

    if(this->_colors.index() == 1) {
      const auto& color = std::get<1>(this->_colors);
      vertices = {
          Vertex{{xpos, ypos + h}, color, {0.0f, 0.0f}},
          Vertex{{xpos, ypos}, color,  {0.0f, 1.0f}},
          Vertex{{xpos + w, ypos}, color, {1.0f, 1.0f}},
          Vertex{{xpos, ypos + h}, color, {0.0f, 0.0f}},
          Vertex{{xpos + w, ypos}, color, {1.0f, 1.0f}},
          Vertex{{xpos + w, ypos + h}, color, {1.0f, 0.0f}}
      };
    } else { // use different color per vertex
      const auto& colors = std::get<0>(this->_colors);
      vertices = {
          Vertex{{xpos, ypos + h}, colors[0], {0.0f, 0.0f}},
          Vertex{{xpos, ypos}, colors[1],  {0.0f, 1.0f}},
          Vertex{{xpos + w, ypos}, colors[2], {1.0f, 1.0f}},
          Vertex{{xpos, ypos + h}, colors[3], {0.0f, 0.0f}},
          Vertex{{xpos + w, ypos}, colors[4], {1.0f, 1.0f}},
          Vertex{{xpos + w, ypos + h}, colors[5], {1.0f, 0.0f}}
      };
    }
    character.texture.bind();
    this->_vbo.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 6, vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // play with pos.x and pos.y to change width and height between characters.
    // TODO: we can add it as function.
    pos.x += character.advance >> 6;
  }
}

void Label::before_draw(const Shader &shader,
                        const std::unique_ptr<Camera> &cam,
                        const std::unique_ptr<LightManager> &lm) noexcept {
  this->_vao.bind();
  shader.use();
  shader.set_float_mat4x4("Model", this->get_model_matrix());
  shader.set_float_mat4x4("View", cam->get_view_matrix());
  shader.set_float_mat4x4("Projection", cam->get_projection_matrix());
  shader.set_bool("FlipVertically", this->_flip_vertically);
  shader.set_bool("FlipHorizontally", this->_flip_horizontally);
  lm->get_point_lights_ssbo().bind();
  lm->update_buffers();
  shader.set_float_vec4("global_ambient_light.color", lm->get_ambient_light().get_color());

  // no different color per vertex
  if (this->_colors.index() == 1) {
    shader.set_float_vec4("TextColor", std::get<1>(this->_colors));
  } else { // different color per vertex, so we use transparent color.
    shader.set_float_vec4("TextColor", detail::drawable::default_color /* this won't change multiplication result */);
  }
}

[[nodiscard]] const std::string& Label::get_current_text() const noexcept {
  return this->_text;
}

void Label::_initialize_fields_other_than_color(
  const Font &font,
  const char *text,
  const glm::vec2 &position,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->_scale = {1.f, 1.f, 0.f};
  this->_font = font;
  this->_text = text;
  this->_position = position;
  this->_rotation_rads = rotation_rads;
  this->_flip_vertically = flip_vertically;
  this->_flip_horizontally = flip_horizontally;

  this->_bbox_w = 0;
  this->_bbox_h = 0;

  for (const auto &ch : this->_text) {
    const auto &glyph = this->_font._face->glyph;
    this->_bbox_w += glyph->advance.x >> 6;
    this->_bbox_h =
        std::max(this->_bbox_h, static_cast<GLfloat>(glyph->bitmap_top));
  }
  this->_relative_pos = glm::vec2{this->_bbox_w / 2.f, this->_bbox_h / 2.f};

  if(this->_vao.get_vao_id() == 0)
    this->_vao.initialize();

  this->_vao.bind();

  if(this->_vbo.get_vbo_id() == 0)
    this->_vbo.empty_initialize(sizeof(Vertex) * 6);

  this->_vbo.bind();

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  // color attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(2 * sizeof(float)));

  // texture coordinate attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(6 * sizeof(float)));
  this->_vbo.unbind();
  this->_vao.unbind();
}
} // namespace fre2d