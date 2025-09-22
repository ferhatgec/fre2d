// MIT License
//
// Copyright (c) 2024-2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "drawable.hpp"
#include <glad/glad.h>

namespace fre2d {
class Polygon : public Drawable {
public:
  Polygon() noexcept = default;
  // specify width and height, then it will automatically apply scale to vertices,
  // so that would make changing size relatively easy.
  // alternatively you can store width, height;
  // then apply them to your vertices and pass by Polygon(vertices, position).
  explicit Polygon(
    GLsizei width,
    GLsizei height,
    const std::vector<Vertex>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  );

  explicit Polygon(
    GLsizei width,
    GLsizei height,
    const std::vector<Vertex2>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  );

  // width and height is 1, so your vertices have to use pixel coordinates.
  explicit Polygon(
    const std::vector<Vertex>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  );

  explicit Polygon(
    const std::vector<Vertex2>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  );

  ~Polygon() override = default;

  void initialize_polygon(
    GLsizei width,
    GLsizei height,
    const std::vector<Vertex>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void initialize_polygon(
    GLsizei width,
    GLsizei height,
    const std::vector<Vertex2>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void initialize_polygon(
    const std::vector<Vertex>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void initialize_polygon(
    const std::vector<Vertex2>& vertices,
    const glm::vec2& position,
    const Texture& texture = Texture::get_default_texture(),
    GLfloat rotation_rads = detail::drawable::default_rotation_radians,
    bool flip_vertically = detail::drawable::default_flip_vertically,
    bool flip_horizontally = detail::drawable::default_flip_horizontally
  ) noexcept;

  void draw(const Shader& shader, const std::unique_ptr<Renderer>& rnd) noexcept override;
  void draw(
      const Shader& shader,
      const std::unique_ptr<Camera>& cam,
      const std::unique_ptr<LightManager>& lm
  ) noexcept override;
private:
  [[nodiscard]] static std::vector<Vertex>
  _apply_tex_coords(const std::vector<Vertex2>& vertices) noexcept;

  [[nodiscard]] static std::tuple<GLfloat, GLfloat, GLfloat, GLfloat>
  _get_aabb(const std::vector<Vertex2>& vertices) noexcept;

  std::size_t _count;
};
} // namespace fre2d