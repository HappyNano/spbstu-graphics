#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>

struct Particle
{
  glm::vec3 pos, vel;
  glm::vec4 color;
  float life;

  Particle();
};

#endif
