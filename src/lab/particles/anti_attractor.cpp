#include "lab/particles/anti_attractor.hpp"

AntiAttractor::AntiAttractor(glm::vec3 pos, float strength):
  _pos{ pos },
  _strength{ strength }
{}

void AntiAttractor::operator()(Particle & particle, float dt)
{
  // Вычисляем вектор отталкивания от точки
  glm::vec3 direction = particle.pos - this->_pos;

  float distance = glm::length(direction);
  // Нормализуем вектор направления (чтобы получить единичный вектор)
  glm::vec3 normalizedDirection = glm::normalize(direction);

  // Вычисляем силу отталкивания (обратно пропорционально квадрату расстояния, либо линейно)
  glm::vec3 repulsion = normalizedDirection * (this->_strength / (distance * distance));

  // Обновляем скорость частицы, добавляя вектор отталкивания
  particle.vel += repulsion * dt;
}
