#ifndef POINT_PARTICLE_GENERATOR_HPP
#define POINT_PARTICLE_GENERATOR_HPP

#include "lab/particles/particle.hpp"

class PointParticleGenerator
{
 public:
  PointParticleGenerator(glm::vec3 pos);

  Particle operator()();

 private:
  glm::vec3 _pos;
};

#endif
