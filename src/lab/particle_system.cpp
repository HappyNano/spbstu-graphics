#include "lab/particle_system.hpp"

#include <iostream> // TODO: Delete

#include <algorithm>
#include <functional>

ParticleSystem::ParticleSystem(Shader::shared shader, size_t amount, glm::vec3 anti_attractor_pos, glm::vec3 pos):
  _particles{ amount },
  _amount{ amount },
  _shader_ptr{ shader },
  _pos{ pos },
  _anti_attractor_pos{ anti_attractor_pos }
{
  // Creating particle vertex buffer (one-point)
  {
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.1f  //
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

void ParticleSystem::update(float dt, size_t newParticles)
{
  // Add new particles
  // -----------------
  for (size_t i = 0; i < newParticles && !_particles.isFull(); ++i)
  {
    this->_particles.makeParticle(_makeParticle());
  }
  // Update all particles
  // --------------------
  for (auto & particle: this->_particles.getAliveParticles())
  {
    particle.old_pos = particle.pos;

    particle.life -= dt;              // reduce life
    particle.vel *= 1.0f + dt / 2.0f; // increasing speed

    // Anti-attractor
    // glm::vec3 between = particle.pos - _anti_attractor_pos;
    // float distance = glm::distance(particle.pos, _anti_attractor_pos);
    // if (distance < _attractor_strength)
    // {
    //   particle.vel = particle.vel - between / 100.f / distance;
    // }
    // Вычисляем вектор отталкивания от точки
    glm::vec3 direction = particle.pos - _anti_attractor_pos;

    float distance = glm::length(direction);
    // Нормализуем вектор направления (чтобы получить единичный вектор)
    glm::vec3 normalizedDirection = glm::normalize(direction);

    // Вычисляем силу отталкивания (обратно пропорционально квадрату расстояния, либо линейно)
    glm::vec3 repulsion = normalizedDirection * (_attractor_strength / (distance * distance));

    // Обновляем скорость частицы, добавляя вектор отталкивания
    particle.vel += repulsion * dt;

    // Обновляем позицию частицы
    particle.pos += particle.vel * dt;

    // Kill particle if particle below y=0.0
    if (particle.pos.y <= 0.0f || particle.pos.y >= 5.0f)
    {
      particle.life = 0.0f;
    }
  }
  _particles.clearDeadParticles();
}

void ParticleSystem::render()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // _shader_ptr->use();
  for (auto && particle: this->_particles.getAliveParticles())
  {
    _shader_ptr->setVec3("old_pos", particle.old_pos);
    _shader_ptr->setVec3("offset", particle.pos);
    glEnable(GL_LINE_SMOOTH);
    glPointSize(5.0f);
    glLineWidth(10.0f);
    glBindVertexArray(this->_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
  }
  // don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Particle ParticleSystem::_makeParticle()
{
  Particle particle;

  float random = ((rand() % 100) - 50) / 1000.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  glm::vec3 rVelocity = { -std::abs((rand() % 100) - 50), (rand() % 100) - 50, 0.0f };
  rVelocity /= 100.0f;

  particle.pos = this->_pos;
  particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.life = 15.0f * (1.0f + (rand() % 100) / 100.0f);
  particle.vel = rVelocity;

  return particle;
}
