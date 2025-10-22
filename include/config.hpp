// MIT License
//
// Copyright (c) 2025 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#define fre2d_newline "\n"

#define fre2d_default_glsl_version "#version 450 core" fre2d_newline

#define fre2d_default_buffer_layouts R"(
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

#define fre2d_default_point_lights_blend_func R"(
vec4 point_lights_blend_func(vec4 Color, float alpha_ch, sampler2D tex, vec2 tex_coords, vec2 frag_pos) {
  for(int i = 0; i < point_lights.length(); i++) {
    Color += mix(
      vec4(calculate_point_light(point_lights[i], tex, tex_coords, frag_pos), alpha_ch),
      vec4(0.f, 0.f, 0.f, 0.f),
      float(point_lights[i].disabled)
    );
  }
  return Color;
}
)" \
fre2d_newline

#define fre2d_default_lighting_fragment  R"(
struct PointLight {
  vec2 pos;
  vec4 ambient;
  vec4 diffuse;

  float constant;
  float linear;
  float quadratic;

  bool disabled; // used in fresh to not calculate lighting
};

struct AmbientLight {
  vec4 color;
};

layout(std430, binding = 0) buffer PointLights {
  PointLight point_lights[];
};

// TODO: we can add bounds to them and therefore we can have multiple ambient lights.
// but right now there is 1 global ambient light and it affects every Drawable
// object.
uniform AmbientLight global_ambient_light;

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
}

vec4 calculate_ambient_light(AmbientLight light) {
  return light.color;
}
)" \
fre2d_newline