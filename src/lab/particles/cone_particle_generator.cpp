#include "lab/particles/cone_particle_generator.hpp"

#include <random>
#include <cmath>

ConeParticleGenerator::ConeParticleGenerator(glm::vec3 center_pos, float height, float radius):
  _center_pos{ center_pos },
  _height{ height },
  _radius{ radius }
{}

ConeParticleGenerator::ConeParticleGenerator(const std::shared_ptr< Cone > & cone):
  ConeParticleGenerator(cone->modelView().get_pos(), cone->get_height(), cone->get_radius())
{}

Particle ConeParticleGenerator::operator()()
{
  float theta = static_cast< float >(rand()) / RAND_MAX * 2.0f * M_PI;
  float y = static_cast< float >(rand()) / RAND_MAX * _height;

  float r_current = _radius * (_height - y) / _height;
  float x = r_current * cos(theta);
  float z = r_current * sin(theta);

  float normal_x = cos(theta);
  float normal_z = sin(theta);
  float normal_y = _radius / _height;
  glm::vec3 normal = glm::vec3(normal_x, normal_y, normal_z);

  return Particle{
    _center_pos + glm::vec3(x, y, z), // Pos
    glm::normalize(normal)            // Vel
  };
}
