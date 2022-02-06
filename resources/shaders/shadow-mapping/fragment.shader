#version 410 core

//precision highp float;

in vec4 light_vertex_position;
in vec4 screen_position;
in vec3 vertex_normal;

out vec3 color;

uniform sampler2DShadow u_light_map;
uniform sampler2D u_previous_framebuffer;
uniform int u_should_add_previous_framebuffer;
uniform vec3 u_light_color;
uniform vec3 u_light_direction;

void main() {

  vec3 normal = vertex_normal;
  if(!gl_FrontFacing) normal = -normal;

  float diffuse = max(0, -dot(normal, u_light_direction));

  color = vec3(0, 0, 0);

  if(diffuse > 0.001) {
    vec3 map_position = light_vertex_position.xyz / light_vertex_position.w;

    map_position.z -= 0.0001;

    float factor = clamp(3 * (1.0 - length(map_position.xy - 0.5) * 2), 0.0, 1.0);
    float depth = texture(u_light_map, map_position) * factor + (1 - factor);

    color = u_light_color * depth * diffuse;
  }

  if(u_should_add_previous_framebuffer == 1) {
    color += texture(u_previous_framebuffer, (screen_position.xy / screen_position.w + 1) * 0.5).rgb;
  }
}
