#include "lab/particle_system.hpp"

#include <iostream>

ParticleSystem::ParticleSystem(Shader::shared shader, size_t amount, glm::vec3 pos):
  _particles{},
  _amount{ amount },
  _shader_ptr{ shader },
  _pos{ pos }
{
  // _particles.reserve(_amount);
  // for (size_t i = 0; i < amount; ++i)
  // {
  //   _particles.emplace_back();
  // }
  // set up mesh and attribute properties
  unsigned int VBO;
  float particle_quad[] = { 0.0f, 0.0f, 0.0f };
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(_VAO);
  // fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
  // set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  _particles.assign(_amount, {});
}

ParticleSystem::~ParticleSystem()
{}

void ParticleSystem::update(float dt, size_t newParticles, glm::vec3 offset)
{
  // add new particles
  for (size_t i = 0; i < newParticles; ++i)
  {
    int unusedParticle = this->_firstUnusedParticle();
    this->_respawnParticle(this->_particles[unusedParticle], offset);
  }
  // update all particles
  for (size_t i = 0; i < this->_amount; ++i)
  {
    Particle & p = this->_particles[i];
    p.life -= dt; // reduce life
    if (p.life > 0.0f)
    { // particle is alive, thus update
      p.pos -= p.vel * dt;
      // p.color.a -= dt * 2.5f;
    }
  }
}

void ParticleSystem::render()
{
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // _shader_ptr->use();
  for (auto && particle: _particles)
  {
    if (particle.life > 0.0f)
    {
      _shader_ptr->setVec3("offset", particle.pos);
      _shader_ptr->setVec4("color", particle.color);
      glPointSize(2.0f);
      glBindVertexArray(this->_VAO);
      glDrawArrays(GL_POINTS, 0, 1);
      glBindVertexArray(0);
      // std::cout << particle.pos.x << ' ' << particle.pos.y << ' ' << particle.pos.z << '\n';
    }
  }
  // don't forget to reset to default blending mode
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

size_t ParticleSystem::_firstUnusedParticle()
{
  static size_t lastUsedParticle = 0;
  // first search from last used particle, this will usually return almost instantly
  for (size_t i = lastUsedParticle; i < this->_amount; ++i)
  {
    if (this->_particles[i].life <= 0.0f)
    {
      lastUsedParticle = i;
      return i;
    }
  }
  // otherwise, do a linear search
  for (size_t i = 0; i < lastUsedParticle; ++i)
  {
    if (this->_particles[i].life <= 0.0f)
    {
      lastUsedParticle = i;
      return i;
    }
  }
  // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
  lastUsedParticle = 0;
  return 0;
}

void ParticleSystem::_respawnParticle(Particle & particle, glm::vec3 offset)
{
  float random = ((rand() % 100) - 50) / 1000.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  glm::vec3 rVelocity = { (rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50 };
  rVelocity /= 100.0f;
  particle.pos = _pos + random + offset;
  particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life = 10.0f * (1.0f + (rand() % 100) / 100.0f);
  particle.vel = rVelocity;
}
