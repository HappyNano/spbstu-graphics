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

GLfloat x, y, z;

void setupViewport(GLFWwindow * window);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void displayCylindre(GLfloat angle);
void displaySphere(GLfloat x, GLfloat y, GLfloat z);

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

  GLfloat angle = 0.0f;
  x = 0.0f;
  y = 1.5f;
  z = 1.5f;

  // Цикл отрисовки
  while (!glfwWindowShouldClose(window))
  {
    // == Начало отрисовки
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f,
     -3.0f,
     4.5f, // Camera position
     0.0f,
     0.0f,
     0.0f, // Look at point
     0.0f,
     1.0f,
     0.0f); // Up vector

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0f, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);

    displayCylindre(sin(angle) * 90);
    angle += 0.001f;
    displaySphere(x, y, z);

    glFlush();
    // == Конец  отрисовки
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void displayCylindre(GLfloat angle)
{
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glColor3f(1.0f, 0.5f, 0.0f);

  GLUquadric * quad = gluNewQuadric();
  glPushMatrix();
  // glTranslatef(0.0f, -3.0f, -3.f); // Перемещаем цилиндр по оси Y
  // glRotatef(90.f, 1.0f, 0.0f, 0.0f); // Поворачиваем цилиндр вокруг оси X, чтобы он был вдоль оси Y
  gluCylinder(quad, 1.0, 1.0, 3.0, 32, 32);
  glPopMatrix();
  gluDeleteQuadric(quad);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void displaySphere(GLfloat x, GLfloat y, GLfloat z)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glColor3f(0.0f, 0.5f, 1.0f);

  GLUquadric * quad = gluNewQuadric();
  glPushMatrix();
  glTranslatef(x, y, z); // Перемещаем сферу по оси Y
  gluSphere(quad, 1.f, 32, 32);
  glPopMatrix();
  gluDeleteQuadric(quad);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
  if (key == GLFW_KEY_Z && action == GLFW_PRESS)
  {
    x -= 0.5f;
  }
  if (key == GLFW_KEY_X && action == GLFW_PRESS)
  {
    y -= 0.5f;
  }
  if (key == GLFW_KEY_C && action == GLFW_PRESS)
  {
    z -= 0.5f;
  }
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
  {
    x += 0.5f;
  }
  if (key == GLFW_KEY_S && action == GLFW_PRESS)
  {
    y += 0.5f;
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
  {
    z += 0.5f;
  }
}
