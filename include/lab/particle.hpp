#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>

struct Particle
{
  glm::vec3 old_pos;
  glm::vec3 pos, vel;
  glm::vec4 color;
  float life;

  Particle();

  bool isDead() const;
};

struct LifeComparator
{
  bool operator()(const Particle &, const Particle &);
};

struct DeadPredicate
{
  bool operator()(const Particle &);
};

#endif
