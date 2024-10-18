#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "lab/particles/particle_container.hpp"

#include "lab/shaders.hpp"
#include "lab/texture.hpp"

namespace
{
  template < typename T >
  concept ParticleFunctor = requires(T a, Particle & p, float dt) {
    {
      a(p, dt)
    } -> std::same_as< void >;
  };
}

class ParticleSystem
{
 public:
  using this_t = ParticleSystem;

  ParticleSystem(Shader::shared shader, size_t amount, glm::vec3 pos = glm::vec3(0.0f));
  ParticleSystem(const this_t &) = delete;
  ParticleSystem(this_t &&) = delete;
  ~ParticleSystem();

  /**
   * @brief Update particles
   * @param dt - delta time
   * @param newParticles - count generating particles
   */
  template < ParticleFunctor Func >
  void update(float dt, size_t newParticles, Func & functor);
  void render();

 private:
  ParticleStorage _particles; ///< Particle Storage
  size_t _amount;             ///< Maximum amount of particles
  Shader::shared _shader_ptr; ///< Pointer to Particle Shader

  unsigned int _VAO; ///< Vertex Array for particle
  glm::vec3 _pos;    ///< Emitter position (Particle's birth position)

  Particle _makeParticle();
  void _makeNewParticles(size_t count);
};

template < ParticleFunctor Func >
void ParticleSystem::update(float dt, size_t newParticles, Func & functor)
{
  _makeNewParticles(newParticles); // Add new particles
  // Update all particles
  // --------------------
  for (auto & particle: this->_particles.getAliveParticles())
  {
    particle.old_pos = particle.pos;

    particle.life -= dt;              // reduce life
    particle.vel *= 1.0f + dt / 2.0f; // increasing speed

    functor(particle, dt);
  }
  _particles.clearDeadParticles();
}

#endif
