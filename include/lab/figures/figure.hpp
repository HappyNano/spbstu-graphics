#ifndef FIGURES_FIGURE_HPP
#define FIGURES_FIGURE_HPP

class Figure
{
 public:
  virtual ~Figure();

  virtual void render() = 0;

 protected:
  Figure() = default;

  unsigned int VAO;
  unsigned int VBO;
};

#endif
