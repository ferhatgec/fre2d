// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <polygon.hpp>
#include <iostream>

namespace fre2d {
Polygon::Polygon(
  GLsizei width,
  GLsizei height,
  const std::vector<Vertex> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) {
  this->initialize_polygon(
    width,
    height,
    vertices,
    position,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally);
}

Polygon::Polygon(
  GLsizei width,
  GLsizei height,
  const std::vector<Vertex2> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) {
  this->initialize_polygon(
    width,
    height,
    vertices,
    position,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

Polygon::Polygon(
  const std::vector<Vertex> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) {
  this->initialize_polygon(
    vertices,
    position,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

Polygon::Polygon(
  const std::vector<Vertex2> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) {
  this->initialize_polygon(
    vertices,
    position,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Polygon::initialize_polygon(
  GLsizei width,
  GLsizei height,
  const std::vector<Vertex> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->_count = vertices.size();
  this->_mesh.initialize(vertices, {}, texture);
  this->initialize_drawable(
    glm::vec3(width, height, 0.f),
    position,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Polygon::initialize_polygon(
  GLsizei width,
  GLsizei height,
  const std::vector<Vertex2> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  const auto vertices3 = Polygon::_apply_tex_coords(vertices);
  this->_count = vertices.size();
  this->_mesh.initialize(vertices3, {}, texture);
  this->initialize_drawable(
    glm::vec3(width, height, 0.f),
    position,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Polygon::initialize_polygon(
  const std::vector<Vertex> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->initialize_polygon(
    1,
    1,
    vertices,
    position,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Polygon::initialize_polygon(
  const std::vector<Vertex2> &vertices,
  const glm::vec2 &position,
  const Texture &texture,
  GLfloat rotation_rads,
  bool flip_vertically,
  bool flip_horizontally
) noexcept {
  this->initialize_polygon(
    1,
    1,
    vertices,
    position,
    texture,
    rotation_rads,
    flip_vertically,
    flip_horizontally
  );
}

void Polygon::draw(const Shader &shader,
                   const std::unique_ptr<Camera> &camera) noexcept {
  this->before_draw(shader, camera);
  shader.use();
  this->_mesh.get_vao().bind();
  this->_mesh.get_vbo().bind();
  glDrawArrays(GL_TRIANGLE_FAN, 0, this->_count);
}

[[nodiscard]] std::vector<Vertex>
Polygon::_apply_tex_coords(const std::vector<Vertex2> &vertices) noexcept {
  std::vector<Vertex> vertices3;
  const auto [
    min_x,
    min_y,
    max_x,
    max_y
  ] = Polygon::_get_aabb(vertices);
  const auto distance_x = max_x - min_x;
  const auto distance_y = max_y - min_y;
  // convert Vertex2 to Vertex3 by applying texture coordinates to it.
  for(const auto& vert: vertices) {
    const auto& pos = vert.get_position();
    vertices3.emplace_back(vert, glm::vec2 {
      (pos.x - min_x) / distance_x, (pos.y - min_y) / distance_y
    });
  }
  return vertices3;
}

// gets minimum bounding rectangle of given convex polygon;
// TODO:
// for concave polygons, there is more complex algorithm out there;
// but we might use triangulation to convert them to small triangles;
// but it seems a bit overkill.
[[nodiscard]] std::tuple<GLfloat, GLfloat, GLfloat, GLfloat>
Polygon::_get_aabb(const std::vector<Vertex2>& vertices) noexcept {
  // since it's convex polygon, we could just take minimums and maximums directly.
  if(vertices.empty()) {
    return { 0.f, 0.f, 0.f, 0.f };
  }
  const auto& first_pos = vertices[0].get_position();
  GLfloat min_x = first_pos.x, min_y = first_pos.y;
  GLfloat max_x = first_pos.x, max_y = first_pos.y;
  for(const auto& vert: vertices) {
    const auto& pos = vert.get_position();
    if(pos.x < min_x) min_x = pos.x;
    if(pos.y < min_y) min_y = pos.y;
    if(pos.x > max_x) max_x = pos.x;
    if(pos.y > max_y) max_y = pos.y;
  }
  return { min_x, min_y, max_x, max_y };
}
} // namespace fre2d