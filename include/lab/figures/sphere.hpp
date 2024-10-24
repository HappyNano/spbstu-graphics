#ifndef FIGURES_SPHERE_HPP
#define FIGURES_SPHERE_HPP

#include "lab/figures/figure.hpp"

class Sphere final: public Figure
{
 public:
  Sphere(float radius);

  float get_radius() const;

 private:
  float _radius;

  size_t _vertices_count = 0;
  size_t _ind_count = 0;
  unsigned int IBO;

  void _render() override;
};

#endif
