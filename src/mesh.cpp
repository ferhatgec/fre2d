// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#include <mesh.hpp>
#include <error.hpp>

namespace fre2d {
Mesh::Mesh(const VertexArray& vao) noexcept {
  this->_vao = vao;
}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           const Texture& texture,
           const VertexArray& vao) noexcept {
  this->_vao = vao;
  this->initialize(vertices, indices, texture);
}

[[nodiscard]] const VertexArray& Mesh::get_vao() const noexcept {
  return this->_vao;
}

[[nodiscard]] const VertexBuffer& Mesh::get_vbo() const noexcept {
  return this->_vbo;
}

[[nodiscard]] const ElementBuffer& Mesh::get_ebo() const noexcept {
  return this->_ebo;
}

[[nodiscard]] const std::optional<Texture>& Mesh::get_texture() const noexcept {
  return this->_texture;
}

void Mesh::initialize(const std::vector<Vertex>& vertices,
                      const std::vector<GLuint>& indices,
                      const Texture& texture) noexcept {
  if(this->get_vao().get_vao_id() == 0) {
    this->_vao.initialize(); // create new vao
  }
  this->_vao.bind();
  this->_vbo.initialize(vertices);
  this->_vbo.bind();
  if(!indices.empty()) {
    this->_ebo.initialize(indices);
    this->_indices = indices;
  }
  this->_vertices = vertices;

  if(texture != Texture::get_default_texture())
    this->_texture = texture;

  // position attribute (x, y)
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute (r, g, b, a)
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coordinate attribute (x, y)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}
} // namespace fre2d