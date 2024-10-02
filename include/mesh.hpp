// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "element_buffer.hpp"
#include "texture.hpp"
#include <optional>

namespace fre2d {
namespace detail::mesh {
// same as empty vao; Mesh will generate new VAO if given VertexArray is default_vertex_array;
// or any VertexArray of ID 0.
static const VertexArray default_vertex_array;
} // namespace fre2d::detail::mesh

// pass vertices and their relative indices
class Mesh {
public:
  explicit Mesh(const VertexArray& vao = detail::mesh::default_vertex_array) noexcept;
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<GLuint>& indices,
       const Texture& texture = Texture::get_default_texture(),
       const VertexArray& vao = detail::mesh::default_vertex_array) noexcept;

  [[nodiscard]] const VertexArray& get_vao() const noexcept;
  [[nodiscard]] const VertexBuffer& get_vbo() const noexcept;
  [[nodiscard]] const ElementBuffer& get_ebo() const noexcept;
  [[nodiscard]] const std::optional<Texture>& get_texture() const noexcept;

  void initialize(const std::vector<Vertex>& vertices,
                  const std::vector<GLuint>& indices,
                  const Texture& texture = Texture::get_default_texture()) noexcept;
private:
  // unless it's explicitly generated, we do not generate new vao per mesh.
  // but copy it.
  VertexArray _vao;
  VertexBuffer _vbo;
  ElementBuffer _ebo;
  std::vector<Vertex> _vertices;
  std::vector<GLuint> _indices;
  std::optional<Texture> _texture;
};
} // namespace fre2d