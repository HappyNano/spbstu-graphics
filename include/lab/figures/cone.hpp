#ifndef FIGURES_CONE_HPP
#define FIGURES_CONE_HPP

#include "lab/figures/figure.hpp"

class Cone final: public Figure
{
 public:
  Cone(float radius, float height);

  float get_radius() const noexcept;
  float get_height() const noexcept;

 private:
  float _radius, _height;

  size_t _vertices_count = 0;

  void _render() override;
};

#endif
