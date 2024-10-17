#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "lab/particle_container.hpp"

#include "lab/shaders.hpp"
#include "lab/texture.hpp"

class ParticleSystem
{
 public:
  using this_t = ParticleSystem;

  ParticleSystem(Shader::shared shader, size_t amount, glm::vec3 anti_attractor_pos, glm::vec3 pos = glm::vec3(0.0f));
  ParticleSystem(const this_t &) = delete;
  ParticleSystem(this_t &&) = delete;
  ~ParticleSystem();

  /**
   * @brief Update particles
   * @param dt - delta time
   * @param newParticles - count generating particles
   */
  // template < typename Func >
  void update(float dt, size_t newParticles);//, Func & functor);
  void render();

 private:
  ParticleStorage _particles; ///< Particle Storage
  size_t _amount;             ///< Maximum amount of particles
  Shader::shared _shader_ptr; ///< Pointer to Particle Shader

  unsigned int _VAO; ///< Vertex Array for particle
  glm::vec3 _pos;    ///< Emitter position (Particle's birth position)

  glm::vec3 _anti_attractor_pos; ///< Anti-Attractor positions
  const float _attractor_strength = 0.4f;

  Particle _makeParticle();
  void _makeNewParticles(size_t count);
};

#endif
