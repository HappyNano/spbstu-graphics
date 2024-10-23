#ifndef FIGURES_FIGURE_HPP
#define FIGURES_FIGURE_HPP

#include "lab/model_view.hpp"

class Figure
{
 public:
  virtual ~Figure();

  auto modelView() -> ModelView &;
  auto modelView() const -> const ModelView &;

  virtual void render() = 0;

 protected:
  Figure() = default;

  ModelView _model_view;

  unsigned int VAO;
  unsigned int VBO;
};

#endif
