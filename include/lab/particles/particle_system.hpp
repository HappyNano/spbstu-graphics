#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "lab/particles/particle_container.hpp"

#include <functional>

#include "lab/shaders.hpp"

namespace
{
  template < typename T >
  concept ParticleFunctor =
   requires(T a, Particle & p, float dt) {
     {
       a(p, dt)
       } -> std::same_as< void >;
   };
}

class ParticleSystem
{
 public:
  using this_t = ParticleSystem;
  using pgenerator_t = std::function< Particle() >;

  ParticleSystem(Shader::shared shader, size_t amount, const pgenerator_t & gen);
  ParticleSystem(const this_t &) = delete;
  ParticleSystem(this_t &&) = delete;
  ~ParticleSystem();

  /**
   * @brief Update particles
   * @param dt - delta time
   * @param newParticles - count generating particles
   */
  template < ParticleFunctor Func >
  void update(float dt, size_t newParticles, Func && functor);
  void render();

  size_t aliveCount() const;

 private:
  ParticleStorage _particles; ///< Particle Storage
  size_t _amount;             ///< Maximum amount of particles
  Shader::shared _shader_ptr; ///< Pointer to Particle Shader
  pgenerator_t _gen;          ///< Particle Generator

  unsigned int _VAO; ///< Vertex Array for particle

  void _makeNewParticles(size_t count);
};

template < ParticleFunctor Func >
void ParticleSystem::update(float dt, size_t newParticles, Func && functor)
{
  _makeNewParticles(newParticles); // Add new particles
  // Update all particles
  // --------------------
  for (auto & particle: this->_particles.getAliveParticles())
  {
    particle.old_pos.push(particle.pos);

    particle.life -= dt; // reduce life

    functor(particle, dt);

    // Обновляем позицию частицы
    particle.pos += particle.vel * dt;
  }
  _particles.clearDeadParticles();
}

#endif
