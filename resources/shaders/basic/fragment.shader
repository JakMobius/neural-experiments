#version 410 core

precision highp float;

in vec3 vertex_color;
in vec4 screen_position;
out vec3 color;

uniform vec3 u_ambient_light;
uniform sampler2D u_shadow_map;

void main() {
  vec2 position = (screen_position.xy / screen_position.w + vec2(1, 1)) * 0.5;
  vec3 light = u_ambient_light + texture(u_shadow_map, position).xyz;

  color = vertex_color * light;
}
