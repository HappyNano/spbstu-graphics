#include "lab/particles/point_particle_generator.hpp"

#include <random>
#include <cmath>

PointParticleGenerator::PointParticleGenerator(glm::vec3 pos):
  _pos{ pos }
{}

Particle PointParticleGenerator::operator()()
{
  Particle particle;

  float random = ((rand() % 100) - 50) / 1000.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  glm::vec3 rVelocity = { -std::abs((rand() % 100) - 50), (rand() % 100) - 50, 0.0f };
  rVelocity /= 100.0f;

  particle.pos = this->_pos;
  particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life = 15.0f * (1.0f + (rand() % 100) / 100.0f);
  particle.vel = rVelocity;

  return particle;
}
