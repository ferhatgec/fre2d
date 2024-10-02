// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <camera.hpp>
#include <error.hpp>
#include <rectangle.hpp>

namespace fre2d {
// TODO: check for double initialization
Rectangle::Rectangle() noexcept {}

Rectangle::Rectangle(GLsizei width,
                     GLsizei height,
                     const glm::vec2& position,
                     const std::array<glm::vec4, 4>& color,
                     const Texture& texture,
                     GLfloat rotation_rads) noexcept {
  this->initialize_rectangle(width, height, position, color, texture, rotation_rads);
}

Rectangle::Rectangle(GLsizei width, GLsizei height, const glm::vec2 &position,
                     const glm::vec4 &color,
                     const Texture& texture,
                     GLfloat rotation_rads) noexcept {
  this->initialize_rectangle(width, height, position, color, texture, rotation_rads);
}

// TODO: check for double initialization; especially Mesh class since we
// generate new VAO instead of passing current one and set buffer attributes one
// more time.
void Rectangle::initialize_rectangle(GLsizei width, GLsizei height,
                           const glm::vec2 &position,
                           const std::array<glm::vec4, 4> &color,
                           const Texture& texture,
                           GLfloat rotation_rads) noexcept {
  this->_vertices = {
    Vertex(glm::vec2(-0.5f, -0.5f), color[0], glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec2(0.5f, -0.5f), color[1], glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec2(0.5f, 0.5f), color[2], glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec2(-0.5f, 0.5f), color[3], glm::vec2(0.0f, 1.0f))
  };

  this->_mesh.initialize(this->_vertices, {0, 1, 2, 2, 3, 0}, texture);
  this->initialize_drawable(glm::vec3(width, height, 0.f), position, rotation_rads);
}

// TODO: check for double initialization
void Rectangle::initialize_rectangle(GLsizei width, GLsizei height,
                           const glm::vec2 &position, const glm::vec4 &color,
                           const Texture& texture,
                           GLfloat rotation_rads) noexcept {
  this->_vertices = {
    Vertex(glm::vec2(-0.5f, -0.5f), color, glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec2(0.5f, -0.5f), color, glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec2(0.5f, 0.5f), color, glm::vec2(0.0f, 1.0f)),
    Vertex(glm::vec2(-0.5f, 0.5f), color, glm::vec2(1.0f, 1.0f))};

  this->_mesh.initialize(this->_vertices, {0, 1, 2, 2, 3, 0}, texture);
  this->initialize_drawable(glm::vec3(width, height, 0.f), position, rotation_rads);
}

void Rectangle::draw(const Shader& shader, const std::unique_ptr<Camera>& camera) noexcept {
  this->before_draw(shader, camera);
  shader.use();
  this->_mesh.get_vao().bind();
  this->_mesh.get_ebo().bind();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
}