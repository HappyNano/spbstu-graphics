#ifndef PARTICLE_CONTAINER_HPP
#define PARTICLE_CONTAINER_HPP

#include <vector>
#include <span>

#include "lab/particle.hpp"

class ParticleStorage final
{
 public:
  using particles_span_t = std::span< Particle >;

  ParticleStorage(size_t amount);
  ParticleStorage(const ParticleStorage &) = delete;
  ParticleStorage(ParticleStorage &&) = delete;
  ~ParticleStorage() = default;

  size_t aliveCount() const;
  bool isFull() const;

  particles_span_t getAliveParticles();

  void makeParticle(const Particle &);
  void clearDeadParticles();

 private:
  using container_t = std::vector< Particle >;

  container_t _particles; ///< Particle Container - std::vector
                          ///< CONST SIZE
  container_t::iterator _death_particle_it;
  size_t _alive_particles;
};

#endif
