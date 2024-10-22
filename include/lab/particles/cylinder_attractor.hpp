#ifndef CYLINDER_ATTRACTOR_HPP
#define CYLINDER_ATTRACTOR_HPP

#include "lab/particles/particle.hpp"

class CylinderAttractor final
{
 public:
  /**
   * @brief Constructor
   * @param pos It's center bottom pos of Cylinder
   * @param radius It's radius of Cylinder
   * @param height It's height of Cylinder
   * @param strength It's strength of attractor
   */
  CylinderAttractor(glm::vec3 pos, float radius, float height, float strength);
  ~CylinderAttractor() = default;

  void operator()(Particle & particle, float dt);

 private:
  glm::vec3 _pos;
  float _radius, _height;
  float _strength;
};

#endif
