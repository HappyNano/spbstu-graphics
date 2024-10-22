#ifndef CONE_PARTICLE_GENERATOR_HPP
#define CONE_PARTICLE_GENERATOR_HPP

#include "lab/particles/particle.hpp"

class ConeParticleGenerator
{
 public:
  ConeParticleGenerator(glm::vec3 center_pos, float height, float radius);

  Particle operator()();

 private:
  glm::vec3 _center_pos; ///< Position of bottom center of cone
  float _height;         ///< Height of cone
  float _radius;         ///< Bottom radius of cone
};

#endif
