#ifndef SPHERE_PARTICLE_GENERATOR_HPP
#define SPHERE_PARTICLE_GENERATOR_HPP

#include "lab/particles/particle.hpp"
#include "lab/figures/sphere.hpp"

class SphereParticleGenerator
{
 public:
  SphereParticleGenerator(glm::vec3 center_pos, float radius);
  SphereParticleGenerator(const std::shared_ptr< Sphere > & sphere);

  Particle operator()();

 private:
  glm::vec3 _center_pos; ///< Position of bottom center of sphere
  float _radius;         ///< Radius of sphere
};

#endif
