#include "lab/particles/surface_attractor.hpp"

#include <algorithm>

SurfaceAttractor::SurfaceAttractor(glm::vec3 pos, float size, float strength):
  _pos{ pos },
  _size{ size },
  _strength{ strength }
{}

SurfaceAttractor::SurfaceAttractor(const std::shared_ptr< Surface > & surface, float strength):
  SurfaceAttractor(surface->modelView().get_pos(), surface->get_size(), strength)
{}

void SurfaceAttractor::operator()(Particle & particle, float dt)
{
  // Половина стороны квадрата плоскости
  float half_size = _size / 2.0f;

  // Притяжение по X и Z к границам квадрата
  float nearest_x = std::clamp(particle.pos.x, -half_size, half_size);
  float nearest_z = std::clamp(particle.pos.z, -half_size, half_size);

  // Вектор направления к ближайшей точке на плоскости
  glm::vec3 nearest_point_on_plane(nearest_x, _pos.y, nearest_z);
  glm::vec3 direction_to_plane = nearest_point_on_plane - particle.pos;

  // Рассчитываем силу притяжения в зависимости от расстояния
  float distance_to_plane = glm::length(direction_to_plane);

  if (distance_to_plane > 0.0f)
  {
    glm::vec3 force_direction = glm::normalize(direction_to_plane);
    float force_magnitude = _strength / (distance_to_plane * distance_to_plane);
    glm::vec3 force = force_magnitude * force_direction;

    // Применяем силу к скорости
    particle.vel += force * dt;
  }
}
