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

  ParticleSystem(Shader shader, Texture2D texture, size_t amount);
  ParticleSystem(const this_t &) = delete;
  ParticleSystem(this_t &&) = delete;
  ~ParticleSystem();

  void update(float dt, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
  void render();

 private:
  std::vector< Particle > _particles;
  size_t _amount;
  Shader _shader;
  Texture2D _texture;

  unsigned int _VAO;
};

#endif
