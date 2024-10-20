#ifndef SPHERE_PARTICLE_GENERATOR_HPP
#define SPHERE_PARTICLE_GENERATOR_HPP

#include "lab/particles/particle.hpp"

class SphereParticleGenerator
{
 public:
  SphereParticleGenerator(glm::vec3 center_pos, float radius);

  Particle operator()();

 private:
  glm::vec3 _center_pos; ///< Position of bottom center of sphere
  float _radius;         ///< Radius of sphere
};

#endif
