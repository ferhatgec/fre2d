// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#define fre2d_newline "\n"

#define fre2d_default_glsl_version "#version 450 core" fre2d_newline

#define default_buffer_layouts R"(
layout (location = 0) in vec2 attr_Position;
layout (location = 1) in vec4 attr_Color;
layout (location = 2) in vec2 attr_TexCoords;
)" \
fre2d_newline

#define fre2d_default_uniforms R"(
/* those uniforms are automatically passed by fre2d */
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform bool FlipVertically;
uniform bool FlipHorizontally;
)" \
fre2d_newline

#define fre2d_default_tex_coords R"(
/* this will avoid unnecessary if statement for FlipVertically and FlipHorizontally */
TexCoords = vec2(
  (1.0 - attr_TexCoords.x) * int(FlipHorizontally) + (attr_TexCoords.x * int(!FlipHorizontally)),
  (1.0 - attr_TexCoords.y) * int(FlipVertically) + (attr_TexCoords.y * int(!FlipVertically))
);
)" \
fre2d_newline

#define fre2d_default_color_func R"(
vec4 calculate_color(vec4 color, sampler2D tex, vec2 tex_coords, bool use_texture) {
  return mix(color, color * texture(tex, tex_coords), float(use_texture));
}
)" \
fre2d_newline

#define fre2d_default_point_light_fragment R"(
struct PointLight {
  vec2 pos;
  vec4 ambient;
  vec4 diffuse;

  float constant;
  float linear;
  float quadratic;

  bool update; // dummy
};

layout(std430, binding = 0) buffer PointLights {
  PointLight point_lights[];
};

vec3 calculate_point_light(PointLight light, sampler2D tex, vec2 tex_coords, vec2 frag_pos) {
  vec3 ambient = vec3(light.ambient);

  vec3 norm = vec3(0.f, 0.f, 1.f);
  vec2 light_dir = vec2(normalize(light.pos - frag_pos));
  float diff = max(dot(norm, vec3(light_dir, 1.f)), 0.f);

  vec3 diffuse = vec3(light.diffuse) * diff;

  float distance = length(light.pos - frag_pos);
  float att = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  ambient *= att;
  diffuse *= att;

  return ambient + diffuse;
})" \
fre2d_newline