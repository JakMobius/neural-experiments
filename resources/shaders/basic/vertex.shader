#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec3 a_color;
layout(location = 4) in int a_transform;

uniform mat4 u_camera_matrix;
uniform samplerBuffer u_matrix_buffer;

out vec3 vertex_color;
out vec4 screen_position;

void main() {
  int matrix_offset = a_transform * 4;

  mat4 object_transform = mat4(
    texelFetch(u_matrix_buffer, matrix_offset),
    texelFetch(u_matrix_buffer, matrix_offset + 1),
    texelFetch(u_matrix_buffer, matrix_offset + 2),
    texelFetch(u_matrix_buffer, matrix_offset + 3)
  );

  vec4 world_position = object_transform * vec4(a_position, 1);

  gl_Position = u_camera_matrix * world_position;
  screen_position = gl_Position;
  vertex_color = a_color;
}
