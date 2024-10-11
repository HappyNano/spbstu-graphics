#include "lab/particle_system.hpp"

ParticleSystem::ParticleSystem(Shader shader, Texture2D texture, size_t amount):
  _particles{},
  _amount{ amount },
  _shader{ shader },
  _texture{ texture }
{
  // _particles.reserve(_amount);
  // for (size_t i = 0; i < amount; ++i)
  // {
  //   _particles.emplace_back();
  // }
  // set up mesh and attribute properties
  unsigned int VBO;
  float particle_quad[] = {
    0.0f, 1.0f, 0.0f, 1.0f, //
    1.0f, 0.0f, 1.0f, 0.0f, //
    0.0f, 0.0f, 0.0f, 0.0f, //

    0.0f, 1.0f, 0.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 0.0f, 1.0f, 0.0f  //
  };
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(_VAO);
  // fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
  // set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindVertexArray(0);

  _particles.assign(_amount);
}

ParticleSystem::~ParticleSystem()
{}

void ParticleSystem::update(float dt, unsigned int newParticles, glm::vec2 offset)
{}

void ParticleSystem::render()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  _shader.use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture.ID);
  // use additive blending to give it a 'glow' effect
  _shader.Use();
  for (auto&& particle: _particles)
  {
    if (particle.Life > 0.0f)
    {
      _shader.setVec3("offset", particle.Position);
      _shader.setVec4("color", particle.Color);
      _texture.bind();
      glBindVertexArray(this->VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  // don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
