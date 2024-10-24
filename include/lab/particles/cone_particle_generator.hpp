#ifndef CONE_PARTICLE_GENERATOR_HPP
#define CONE_PARTICLE_GENERATOR_HPP

#include "lab/particles/particle.hpp"

#include "lab/figures/cone.hpp"

class ConeParticleGenerator
{
 public:
  ConeParticleGenerator(glm::vec3 center_pos, float height, float radius);
  ConeParticleGenerator(const std::shared_ptr< Cone > & cone);

  Particle operator()();

 private:
  glm::vec3 _center_pos; ///< Position of bottom center of cone
  float _height;         ///< Height of cone
  float _radius;         ///< Bottom radius of cone
};

#endif
