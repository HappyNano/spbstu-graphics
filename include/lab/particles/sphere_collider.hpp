#ifndef SPHERE_COLLIDER_HPP
#define SPHERE_COLLIDER_HPP

#include "lab/particles/particle.hpp"

#include "lab/figures/sphere.hpp"

class SphereCollider final
{
 public:
  SphereCollider(glm::vec3 center_pos, float radius);
  SphereCollider(const std::shared_ptr< Sphere > & sphere);
  ~SphereCollider() = default;

  void operator()(Particle & particle, float dt);

 private:
  glm::vec3 _center_pos;
  float _radius; ///< Side length
};

#endif
