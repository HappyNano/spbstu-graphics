#include "lab/particle_container.hpp"

#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <iostream>

ParticleStorage::ParticleStorage(size_t amount):
  _particles{ amount, Particle{} },
  _death_particle_it{ _particles.begin() },
  _alive_particles{ 0 }
{}

size_t ParticleStorage::aliveCount() const
{
  return _alive_particles;
}

bool ParticleStorage::isFull() const
{
  return aliveCount() == _particles.size();
}

typename ParticleStorage::particles_span_t ParticleStorage::getAliveParticles()
{
  return std::span(_particles.begin(), _alive_particles);
}

void ParticleStorage::makeParticle(const Particle & new_particle)
{
  if (isFull())
  {
    throw std::logic_error("Full!");
  }
  (*_death_particle_it) = std::move(new_particle);
  auto insert_it = std::lower_bound(this->_particles.begin(), _death_particle_it, new_particle, LifeComparator{});
  while (insert_it != _death_particle_it)
  {
    std::iter_swap(insert_it, _death_particle_it);
    ++insert_it;
  }
  ++_death_particle_it;
  ++_alive_particles;
}

void ParticleStorage::clearDeadParticles()
{
  auto new_death_particle_it = std::remove_if(_particles.begin(), _death_particle_it, DeadPredicate{});
  _alive_particles -= std::distance(new_death_particle_it, _death_particle_it);
  _death_particle_it = new_death_particle_it;
}
