#include "lab/particles/point_particle_generator.hpp"

#include <random>
#include <cmath>

PointParticleGenerator::PointParticleGenerator(glm::vec3 pos):
  _pos{ pos }
{}

Particle PointParticleGenerator::operator()()
{
  glm::vec3 rVelocity = { -std::abs((rand() % 100) - 50), (rand() % 100) - 50, 0.0f };

  return Particle(this->_pos, rVelocity / 100.0f);
}
