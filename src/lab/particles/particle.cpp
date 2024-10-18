#include "lab/particles/particle.hpp"

Particle::Particle():
  pos(0.0f),
  vel(0.0f),
  color(1.0f),
  life(0.0f)
{}

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
