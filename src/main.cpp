#include <iostream>
#include <functional>
#include <string>

// Подключение GLEW до GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <GLFW/glfw3.h>

#include <math.h>
#include <unistd.h>

#include "lab1/settings.hpp"

void check(bool error, const std::string & msg, std::function< void(void) > todo = {}) noexcept(false)
{
  if (error)
  {
    todo();
    throw std::logic_error(msg);
  }
}

float randf()
{
  return static_cast< float >(rand()) / static_cast< float >(RAND_MAX);
}

void setupViewport(GLFWwindow * window);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void displayTeapot(GLdouble x = 0.0, GLdouble y = 0.0, GLdouble z = 5.0);

int main(int argc, char ** argv)
{
  // == Инициализация GLUT
  glutInit(&argc, argv);

  // == Инициализация GLFW
  check(!glfwInit(), "GLFW Bad initialization");

  // Окно
  GLFWwindow * window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Lab1", nullptr, nullptr);
  check(window == nullptr, "Failed to create GLFW window", []() { glfwTerminate(); });
  glfwMakeContextCurrent(window);

  // == Инициализация GLEW
  glewExperimental = GL_TRUE;
  check(glewInit() != GLEW_OK, "Failed to initialize GLEW");

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  setupViewport(window);

  // Программа

  glfwSetKeyCallback(window, key_callback);

  GLdouble x = 0.0;
  GLdouble y = 0.0;
  GLdouble z = 5;

  // Цикл отрисовки
  while (!glfwWindowShouldClose(window))
  {
    // == Начало отрисовки
    displayTeapot(sin(x) * 10, cos(y) * 1, sin(z + M_PI) * 5); // Вызов функции отображения
    x += 0.005;
    y += 0.005;
    z += 0.005;
    // == Конец  отрисовки
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void displayTeapot(GLdouble x, GLdouble y, GLdouble z)
{
  // Clear the color buffer and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the view matrix (camera)
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x,
   y,
   z, // Camera position
   0.0f,
   0.0f,
   0.0f, // Look at point
   0.0f,
   1.0f,
   0.0f); // Up vector

  // Set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Render a solid teapot
  glColor3f(1.0f, 0.5f, 0.0f); // Orange color for the teapot
  glutSolidTeapot(1.0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Swap the buffers (double buffering)
  glFlush();
}

void setupViewport(GLFWwindow * window)
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  // Нужно передать OPENGL размеры нашего окна
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}
