#ifndef CUBE_COLLIDER_HPP
#define CUBE_COLLIDER_HPP

#include "lab/particles/particle.hpp"

#include "lab/figures/cube.hpp"

class CubeCollider final
{
 public:
  CubeCollider(Cube& cube, glm::vec3 center_pos, float length);
  ~CubeCollider() = default;
  
  void operator()(Particle & particle, float dt);

 private:
  Cube& _cube;
  glm::vec3 _center_pos;
  float _length; ///< Side length
};

#endif
