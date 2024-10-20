#ifndef CYLINDRE_PARTICLE_GENERATOR_HPP
#define CYLINDRE_PARTICLE_GENERATOR_HPP

#include "lab/particles/particle.hpp"

class CylindreParticleGenerator
{
 public:
  CylindreParticleGenerator(glm::vec3 center_pos, float height, float radius);

  Particle operator()();

 private:
  glm::vec3 _center_pos; ///< Position of bottom center of cylindre
  float _height; ///< Height of cylindre
  float _radius; ///< Radius of cylindre
};

#endif
