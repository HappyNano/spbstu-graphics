#include "lab/particle_system.hpp"

// #include <iostream>
#include <algorithm>
#include <functional>

ParticleSystem::ParticleSystem(Shader::shared shader, size_t amount, glm::vec3 pos):
  _particles{},
  _amount{ amount },
  _shader_ptr{ shader },
  _pos{ pos }
{
  // Creating particle vertex buffer (one-point)
  {
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
  }
}

ParticleSystem::~ParticleSystem()
{}

void ParticleSystem::update(float dt, size_t newParticles)
{
  // Add new particles
  // -----------------
  for (size_t i = 0; i < newParticles && _particles.size() < _amount; ++i)
  {
    Particle new_particle = _makeParticle();
    auto insert_it = std::upper_bound(this->_particles.cbegin(), this->_particles.cend(), new_particle, LifeComparator{});
    this->_particles.insert(insert_it, new_particle);
  }
  // Update all particles
  // --------------------
  for (auto & particle: _particles)
  {
    particle.life -= dt; // reduce life
    particle.pos -= particle.vel * dt;
    particle.vel *= 1.0f + dt / 2.0f; // increasing speed
    // particle.color.a -= dt * 2.5f;
  }
  _removeDeathParticles();
}

void ParticleSystem::render()
{
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // _shader_ptr->use();
  for (auto && particle: this->_particles)
  {
    if (!particle.isDead())
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

Particle ParticleSystem::_makeParticle()
{
  Particle particle;

  float random = ((rand() % 100) - 50) / 1000.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  glm::vec3 rVelocity = { (rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50 };
  rVelocity /= 100.0f;

  particle.pos = this->_pos + random;
  particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life = 10.0f * (1.0f + (rand() % 100) / 100.0f);
  particle.vel = rVelocity;

  return particle;
}

void ParticleSystem::_removeDeathParticles()
{
  for (auto && particle: this->_particles)
  {
    if (particle.isDead())
    {
      _particles.pop_front();
    }
  }
}
