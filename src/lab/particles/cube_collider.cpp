#include "lab/particles/cube_collider.hpp"

#include <algorithm>
#include <optional>
#include <limits>

namespace
{
  std::optional< glm::vec3 > linePlaneIntersection(glm::vec3 ray, glm::vec3 rayOrigin, glm::vec3 normal, glm::vec3 coord)
  {
    float d = glm::dot(normal, coord);

    if (glm::dot(normal, ray) == 0)
    {
      return std::nullopt; // No intersection, the line is parallel to the plane
    }
    // Compute the X value for the directed line ray intersecting the plane
    float x = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, ray);
    return rayOrigin + glm::normalize(ray) * x;
  }
}

CubeCollider::CubeCollider(const std::shared_ptr< Cube > & cube):
  _cube_ptr{ cube },
  _center_pos{ cube->modelView().get_pos() },
  _length{ cube->getLength() }
{}

void CubeCollider::operator()(Particle & particle, float dt)
{
  float half_size = _length / 2.0f;

  glm::vec3 last_pos = particle.old_pos.front();

  glm::vec3 dx(1.0f, 0.0f, 0.0f), dy(0.0f, 1.0f, 0.0f), dz(0.0f, 0.0f, 1.0f);
  glm::vec3 d = particle.pos - _center_pos;
  bool inside = glm::abs(glm::dot(d, dx)) <= half_size && glm::abs(glm::dot(d, dy)) <= half_size && glm::abs(glm::dot(d, dz)) <= half_size;
  if (inside)
  {
    glm::vec3 plane_normal;
    float min_dist = std::numeric_limits< float >::max();
    for (auto && plane: _cube_ptr->getPlanes())
    {
      auto res = linePlaneIntersection(particle.pos - last_pos, last_pos, plane.normal, plane.pos + _center_pos);
      if (res.has_value())
      {
        float dist = glm::distance(*res, last_pos);
        if (dist < min_dist)
        {
          min_dist = dist;
          plane_normal = plane.normal;
        }
      }
    }
    particle.vel = glm::reflect(particle.vel, plane_normal); // отразить скорость
    particle.pos = last_pos;
  }
}
