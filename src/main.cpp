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
#include "lab1/material.hpp"

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
  x = 1.0f;
  y = 1.0f;
  z = 1.0f;

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

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define light properties
    GLfloat light_position[] = { x, y, z, 1.0f };          // Position of the light
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // Ambient light
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // Diffuse light
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Specular light

    // Set light properties
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

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
  glutWireCylinder(0.5f, 1.0f, 32, 32); // Радиус 0.5, высота 1.0
  glPopMatrix();
}

void displaySphere()
{
  glColor3f(0.0f, 0.5f, 1.0f);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.0f); // Центр сферы на оси цилиндра
  glutWireSphere(0.5f, 32, 32);   // Радиус 0.5
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

void setMaterial(const MaterialConf & material)
{
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
}

void displayTor()
{
  glPushAttrib(GL_LIGHTING_BIT);
  glPushMatrix();

  glEnable(GL_BLEND); // Enable blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  setMaterial(MaterialConf{ { 0.5f, 0.2f, 0.7f, 0.3f }, { 0.1f, 0.1f, 0.1f, 0.3f }, { 1.0f, 1.0f, 1.0f, 0.3f }, { 10.0f } });

  glTranslatef(0.3f, 0.0f, 1.0f);
  glRotatef(-10, 1.0f, 1.0f, 0.0f);
  glutSolidTorus(0.1f, 0.2f, 32, 32);

  glPopMatrix();
  glPopAttrib();
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
