#include "lab/particles/sphere_collider.hpp"

#include <algorithm>
#include <optional>
#include <limits>

SphereCollider::SphereCollider(glm::vec3 center_pos, float radius):
  _center_pos{ center_pos },
  _radius{ radius }
{}

void SphereCollider::operator()(Particle & particle, float dt)
{
  glm::vec3 last_pos = particle.old_pos.front();

  if (glm::distance(particle.pos, _center_pos) <= _radius)
  {
    particle.vel = glm::reflect(particle.vel, glm::normalize(particle.pos - _center_pos)); // отразить скорость
    particle.pos = last_pos;
  }
}
