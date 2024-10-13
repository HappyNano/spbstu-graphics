#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <deque>

#include "lab/particle.hpp"
#include "lab/shaders.hpp"
#include "lab/texture.hpp"

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
  void update(float dt, size_t newParticles);
  void render();

 private:
  std::deque< Particle > _particles; ///< Particle Storage
  size_t _amount;                    ///< Maximum amount of particles
  Shader::shared _shader_ptr;        ///< Pointer to Particle Shader

  unsigned int _VAO; ///< Vertex Array for particle
  glm::vec3 _pos;    ///< Emitter position (Particle's birth position)

  Particle _makeParticle();
  void _removeDeathParticles();
};

#endif
