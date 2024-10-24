#ifndef FIGURES_FIGURE_HPP
#define FIGURES_FIGURE_HPP

#include "lab/model_view.hpp"
#include "lab/shaders.hpp"

class Figure
{
 public:
  virtual ~Figure();

  auto modelView() -> ModelView &;
  auto modelView() const -> const ModelView &;

  void render();
  void render_modelView(Shader & shader);

 protected:
  Figure() = default;

  ModelView _model_view;

  unsigned int VAO;
  unsigned int VBO;

  virtual void _render() = 0;
};

#endif
