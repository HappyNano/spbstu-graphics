#include "lab/particles/particle.hpp"

Particle::Particle():
  Particle(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec4(1.0f), 0.0f)
{}

Particle::Particle(glm::vec3 pos, glm::vec3 vel, glm::vec4 color, float life, size_t trace_length):
  old_pos(),
  pos(pos),
  vel(vel),
  color(color),
  life(life)
{
  for (size_t i = 0; i < trace_length - 1; ++i)
  {
    old_pos.emplace(pos);
  }
}

void Particle::set_traceLength(size_t trace_length)
{
  auto new_queue = std::queue< glm::vec3 >();
  for (size_t i = 0; i < trace_length - 1; ++i)
  {
    new_queue.emplace(pos);
  }
  old_pos = std::move(new_queue);
}

bool Particle::isDead() const
{
  return this->life <= 0.0f;
}

void Particle::kill()
{
  this->life = 0.0f;
}

bool LifeComparator::operator()(const Particle & p1, const Particle & p2)
{
  return p1.life < p2.life;
}

bool DeadPredicate::operator()(const Particle & p)
{
  return p.isDead();
}
