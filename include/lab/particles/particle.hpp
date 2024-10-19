#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>
#include <queue>

struct Particle
{
  std::queue< glm::vec3 > old_pos;
  glm::vec3 pos, vel;
  glm::vec4 color;
  float life;

  Particle();
  Particle(glm::vec3 pos, glm::vec3 vel, glm::vec4 color, float life, size_t trace_length = 1);

  bool isDead() const;
  void kill();
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
