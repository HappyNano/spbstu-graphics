#ifndef CUBE_COLLIDER_HPP
#define CUBE_COLLIDER_HPP

#include "lab/particles/particle.hpp"

#include "lab/figures/cube.hpp"

class CubeCollider final
{
 public:
  CubeCollider(const std::shared_ptr< Cube > & cube);
  ~CubeCollider() = default;

  void operator()(Particle & particle, float dt);

 private:
  std::shared_ptr< Cube > _cube_ptr;
  glm::vec3 _center_pos;
  float _length; ///< Side length
};

#endif
