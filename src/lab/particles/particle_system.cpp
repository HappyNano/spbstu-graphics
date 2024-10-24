#include "lab/particles/particle_system.hpp"

#include <algorithm>
#include <functional>

ParticleSystem::ParticleSystem(Shader::shared shader, size_t amount, const pgenerator_t & gen):
  _particles{ amount },
  _amount{ amount },
  _shader_ptr{ shader },
  _gen{ gen }
{
  // Creating particle vertex buffer (one-point)
  {
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.2f  //
    };
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(_VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}

ParticleSystem::~ParticleSystem()
{}

void ParticleSystem::render()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // _shader_ptr->use();
  for (auto && particle: this->_particles.getAliveParticles())
  {
    if (!particle.old_pos.empty())
    {
      _shader_ptr->setVec3("old_pos", particle.old_pos.front());
      particle.old_pos.pop();
    }
    _shader_ptr->setVec3("offset", particle.pos);
    _shader_ptr->setFloat("alpha", std::min(particle.life / 3.0f, 1.0f));
    glEnable(GL_LINE_SMOOTH);
    glBindVertexArray(this->_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
  }
  // don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleSystem::_makeNewParticles(size_t count)
{
  for (size_t i = 0; i < count && !_particles.isFull(); ++i)
  {
    this->_particles.makeParticle(_gen());
  }
}

size_t ParticleSystem::aliveCount() const
{
  return this->_particles.aliveCount();
}
