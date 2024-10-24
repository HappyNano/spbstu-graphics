#include "lab/particles/cylindre_particle_generator.hpp"

#include <random>
#include <cmath>

CylindreParticleGenerator::CylindreParticleGenerator(glm::vec3 center_pos, float height, float radius):
  _center_pos{ center_pos },
  _height{ height },
  _radius{ radius }
{}

Particle CylindreParticleGenerator::operator()()
{
  float theta = static_cast< float >(rand()) / RAND_MAX * 2.0f * M_PI;
  float y = static_cast< float >(rand()) / RAND_MAX * _height;
  float x = _radius * std::cos(theta);
  float z = _radius * std::sin(theta);

  float normal_x = std::cos(theta);
  float normal_z = std::sin(theta);

  return Particle{
    glm::vec3(x, y, z) + _center_pos, // Pos
    { normal_x, 0.0f, normal_z }      // Vel
  };
}
