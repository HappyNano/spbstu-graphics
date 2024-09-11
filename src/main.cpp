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
float fov = 45;

void setupViewport(GLFWwindow * window);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  fov -= (float)yoffset;
  if (fov < 1.0f)
  {
    fov = 1.0f;
  }
  if (fov > 90.0f)
  {
    fov = 90.0f;
  }
}
void displayCylindre();
void displaySphere();
void displayCube();
void displayTor();

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
  glfwSetScrollCallback(window, scroll_callback);

  GLfloat angle = 0.0f;
  x = 0.0f;
  y = 0.5f;
  z = 4.0f;

  // Цикл отрисовки
  while (!glfwWindowShouldClose(window))
  {
    // == Начало отрисовки
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(0,
    //  0,
    //  0, // Camera position
    //  0.0f,
    //  0.0f,
    //  -1.0f, // Look at point
    //  0.0f,
    //  1.0f,
    //  0.0f); // Up vector

    // // Set up the projection matrix
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(90, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 300.0f);

    displayCylindre();
    displaySphere();
    displayCube();
    displayTor();

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 1000.0f);
    glTranslatef(0.0f, -0.25f, -2.0f);

    glFlush();
    // == Конец  отрисовки
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void displayCylindre()
{
  static GLfloat angle = 0.0f;
  glColor3f(1.0f, 0.5f, 0.0f);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.0f);       // Центр в начале координат
  glRotatef(90, 1.0f, 0.0f, 0.0f);      // Цилиндр вертикально
  glutWireCylinder(0.5f, 1.0f, 16, 16); // Радиус 0.5, высота 1.0
  glPopMatrix();
}

void displaySphere()
{
  glColor3f(0.0f, 0.5f, 1.0f);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.0f); // Центр сферы на оси цилиндра
  glutWireSphere(0.5f, 16, 16);   // Радиус 0.5
  glPopMatrix();
}

void displayCube()
{
  glColor3f(0.0f, 0.7f, 0.7f);
  glPushMatrix();
  glTranslatef(-1.0f, 0.0f, 0.0f);
  glScalef(0.7f, 0.7f, 0.7f);
  glutSolidCube(0.5f);
  glPopMatrix();
}

void displayTor()
{
  glColor3f(0.2f, 0.5f, 0.5f);
  glPushMatrix();
  glRotatef(-10, 1.0f, 1.0f, 0.0f);
  glTranslatef(1.1f, 0.0f, 0.0f);
  glutSolidTorus(0.15f, 0.3f, 16, 16);
  glPopMatrix();
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
  const float cameraSpeed = 0.05f; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    z += cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    z -= cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    x -= cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    x += cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
  {
    y -= cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
  {
    y += cameraSpeed;
  }
}
