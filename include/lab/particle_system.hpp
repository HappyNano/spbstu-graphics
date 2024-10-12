#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <vector>

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

  void update(float dt, size_t newParticles, glm::vec3 offset = glm::vec3(0.0f));
  void render();

 private:
  std::vector< Particle > _particles;
  size_t _amount;
  Shader::shared _shader_ptr;

  unsigned int _VAO;
  glm::vec3 _pos;

  size_t _firstUnusedParticle();
  void _respawnParticle(Particle & particle, glm::vec3 offset);
};

#endif
