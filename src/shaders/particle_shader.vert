#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec3 old_pos;

void main()
{
  float scale = 10.0f;
  ParticleColor = color;
  if (position.x == 1.0f)
  {
    gl_Position = projection * view * vec4(old_pos, 1.0);
  }
  else
  {
    gl_Position = projection * view * vec4(position + offset, 1.0);
  }
}