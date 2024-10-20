#include "lab/particles/sphere_particle_generator.hpp"

#include <random>
#include <cmath>

SphereParticleGenerator::SphereParticleGenerator(glm::vec3 center_pos, float radius):
  _center_pos{ center_pos },
  _radius{ radius }
{}

Particle SphereParticleGenerator::operator()()
{
  float rColor = 0.5f + ((rand() % 100) / 100.0f);

  // Генерация случайных углов
  float theta = static_cast< float >(rand()) / RAND_MAX * 2.0f * M_PI; // Угол азимута от 0 до 2π
  float phi = static_cast< float >(rand()) / RAND_MAX * M_PI;          // Угол высоты от 0 до π

  glm::vec3 point = { _radius * sin(phi) * cos(theta), _radius * cos(phi), _radius * sin(phi) * sin(theta) };

  return Particle{
    point,                                   // Pos
    glm::normalize(point - _center_pos),     // Vel
    glm::vec4(rColor, rColor, rColor, 1.0f), // Color
    3.0f * (1.0f + (rand() % 100) / 100.0f)  // Life
  };
}
