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
  Particle(glm::vec3 pos, glm::vec3 vel, glm::vec4 color = glm::vec4{ 1.0f }, float life = 1.0f, size_t trace_length = 1);

  void set_traceLength(size_t trace_length);

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
