#version 330 core
layout (location = 0) in vec3 position;

out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;

void main()
{
  float scale = 10.0f;
  ParticleColor = color;
  gl_Position = projection * view * mat4(1.0) * vec4(position + offset, 1.0);
}