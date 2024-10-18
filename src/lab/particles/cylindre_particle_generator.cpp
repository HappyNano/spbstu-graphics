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
  Particle particle;

  float rColor = 0.5f + ((rand() % 100) / 100.0f);

  float theta = static_cast< float >(rand()) / RAND_MAX * 2.0f * M_PI;
  float y = static_cast< float >(rand()) / RAND_MAX * _height;
  float x = _radius * std::cos(theta);
  float z = _radius * std::sin(theta);

  float normal_x = std::cos(theta);
  float normal_z = std::sin(theta);

  particle.pos = glm::vec3(x, y, z) + _center_pos;
  particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life = 3.0f * (1.0f + (rand() % 100) / 100.0f);
  particle.vel = { normal_x, 0.0f, normal_z };

  return particle;
}
