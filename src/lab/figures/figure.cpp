#include "lab/figures/figure.hpp"

#include "glad/glad.h"

Figure::~Figure()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

auto Figure::modelView() -> ModelView &
{
  return _model_view;
}
auto Figure::modelView() const -> const ModelView &
{
  return _model_view;
}

void Figure::render()
{
  _render();
}

void Figure::render_modelView(Shader & shader)
{
  shader.setMat4("model", _model_view.get_modelView());
  _render();
}
