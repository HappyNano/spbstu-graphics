#ifndef ATNI_ATTRACTOR_HPP
#define ATNI_ATTRACTOR_HPP

#include "lab/particles/particle.hpp"

class AntiAttractor final
{
 public:
  AntiAttractor(glm::vec3 pos, float strength);
  ~AntiAttractor() = default;
  
  void operator()(Particle & particle, float dt);

 private:
  glm::vec3 _pos;
  float _strength;
};

#endif
