#include "lab/particles/cylinder_attractor.hpp"

#include <algorithm>

CylinderAttractor::CylinderAttractor(glm::vec3 pos, float radius, float height, float strength):
  _pos{ pos },
  _radius{ radius },
  _height{ height },
  _strength{ strength }
{}

CylinderAttractor::CylinderAttractor(const std::shared_ptr< Cylindre > & cylinder, float strength):
  CylinderAttractor(cylinder->modelView().get_pos(), cylinder->get_radius(), cylinder->get_height(), strength)
{}

void CylinderAttractor::operator()(Particle & particle, float dt)
{
  // Проекция позиции частицы на ось цилиндра (ось Y)
  glm::vec3 particleToCenter = particle.pos - _pos;

  // Ограничиваем высоту частицы на высоту цилиндра
  float y_clamped = glm::clamp(particleToCenter.y, 0.0f, _height);

  // Найдем ближайшую точку на цилиндре (по радиусу)
  glm::vec3 nearestPoint = _pos + glm::vec3(0.0f, y_clamped, 0.0f);

  // Проекция позиции на плоскость XZ для расчета радиуса
  glm::vec2 particleXZ(particle.pos.x - _pos.x, particle.pos.z - _pos.z);
  if (glm::length(particleXZ) > _radius)
  {
    particleXZ = glm::normalize(particleXZ) * _radius;
  }

  // Обновляем ближайшую точку на цилиндре с учетом радиуса
  nearestPoint.x = _pos.x + particleXZ.x;
  nearestPoint.z = _pos.z + particleXZ.y;

  // Рассчитываем силу притяжения
  glm::vec3 direction = nearestPoint - particle.pos;
  float distance = glm::length(direction);

  if (distance > 0.001f)
  { // проверка на очень малое расстояние
    // Нормализуем вектор силы и применяем силу
    glm::vec3 force = _strength * (direction / (distance * distance)); // сила уменьшается с расстоянием

    // Применяем силу к скорости частицы
    particle.vel += force * dt;
  }
}
