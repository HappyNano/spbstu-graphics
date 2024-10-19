#ifndef SURFACE_ATTRACTOR_HPP
#define SURFACE_ATTRACTOR_HPP

#include "lab/particles/particle.hpp"

class SurfaceAttractor final
{
 public:
  SurfaceAttractor(glm::vec3 pos, float size, float strength);
  ~SurfaceAttractor() = default;
  
  void operator()(Particle & particle, float dt);

 private:
  glm::vec3 _pos;
  float _size;
  float _strength;
};

#endif
