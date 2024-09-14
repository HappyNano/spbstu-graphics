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
#include "lab1/texture.hpp"

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
GLfloat r, g, b;
float fov = 45;

GLuint textureID;

void setMaterial(const MaterialConf & material)
{
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
}

void setupViewport(GLFWwindow * window);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);

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
  glfwSetCursorPosCallback(window, cursor_position_callback);

  GLfloat angle = 0.0f;
  x = 1.0f;
  y = 1.0f;
  z = 1.0f;
  r = 1.0f;
  g = 1.0f;
  b = 1.0f;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  textureID = loadTexture("assets/redstone_block.bmp");

  // Цикл отрисовки
  while (!glfwWindowShouldClose(window))
  {
    // == Начало отрисовки
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 1000.0f);
    glTranslatef(0.0f, -0.25f, -2.0f);

    glEnable(GL_DEPTH_TEST);
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define light properties
    GLfloat light_position[] = { x, y, z, 1.0f };          // Position of the light
    GLfloat light_ambient[] = { r, g, b, 1.0f };           // Ambient light
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // Diffuse light
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Specular light

    // Set light properties
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    displaySphere();
    displayTor();
    displayCube();
    displayCylindre();

    glFlush();
    // == Конец  отрисовки
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteTextures(1, &textureID);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void displayCylindre()
{
  glPushAttrib(GL_LIGHTING_BIT);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  setMaterial(MaterialConf{ { 0.2f, 0.2f, 0.2f, 0.4f }, { 0.1f, 0.1f, 0.1f, 0.4f }, { 0.0f, 0.0f, 0.0f, 0.4f }, { 5.0f } });

  glTranslatef(0.0f, 0.0f, 0.0f);
  glRotatef(90, 1.0f, 0.0f, 0.0f);
  glutSolidCylinder(0.5f, 1.0f, 128, 128);

  glPopMatrix();
  glPopAttrib();
}

void displaySphere()
{
  glPushAttrib(GL_LIGHTING_BIT);
  glPushMatrix();

  setMaterial(MaterialConf{ { 0.0f, 0.7f, 0.3f, 1.0f }, { 0.1f, 0.1f, 0.1f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 5.0f } });
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.5f, 128, 128);

  glPopMatrix();
  glPopAttrib();
}

void displayCube()
{
  static GLfloat angle = 0.0f;
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  glPushMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // glColor4f(0.8f, 0.1f, 0.1f, 1.0f);

  setMaterial(MaterialConf{ { 0.7f, 0.4f, 0.1f, 1.0f }, { 0.2f, 0.1f, 0.1f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 5.0f } });

  glRotatef((sin(angle += 0.001) + 1) * 45.f, 1.0f, 0.0f, 0.0f);
  glTranslatef(-1.0f, 0.0f, 0.0f);
  glScalef(0.5f, 0.5f, 0.5f);

  GLfloat texLeft = 0.0f;
  GLfloat texRight = 1.0f;
  GLfloat texBottom = 0.0f;
  GLfloat texTop = 1.0f;
  glBegin(GL_QUADS);
  // Front face
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(texRight, texBottom);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(texRight, texTop);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  // Back face
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(texRight, texBottom);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glTexCoord2f(texRight, texTop);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  // Left face
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(texRight, texBottom);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(texRight, texTop);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  // Right face
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glTexCoord2f(texRight, texBottom);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(texRight, texTop);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(0.5f, 0.5f, -0.5f);
  // Top face
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(texRight, texBottom);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(texRight, texTop);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  // Bottom face
  glTexCoord2f(texLeft, texBottom);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(texRight, texBottom);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glTexCoord2f(texRight, texTop);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(texLeft, texTop);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
  glPopAttrib();
}

void displayTor()
{
  glPushAttrib(GL_LIGHTING_BIT);
  glPushMatrix();

  setMaterial(MaterialConf{ { 0.5f, 0.2f, 0.7f, 1.0f }, { 0.1f, 0.1f, 0.1f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 5.0f } });

  glTranslatef(0.3f, 0.2f, 1.0f);
  glRotatef(20, 0.0f, 0.0f, 1.0f);
  glRotatef(20, 0.0f, 1.0f, 0.0f);
  glRotatef(-20, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.3f, 0.2f, -2.0f);
  glutSolidTorus(0.3f, 0.6f, 128, 128);

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
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    z -= cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    z += cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
  {
    r = 1.0f;
    g = 0.0f;
    b = 0.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
  {
    r = 0.0f;
    g = 1.0f;
    b = 0.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
  {
    r = 0.0f;
    g = 0.0f;
    b = 1.0f;
  }
}

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

void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
  // std::cout << xpos << ' ' << ypos << '\n';
  x = (xpos / W_WIDTH - 1 / 2.0f) * 0.8;
  y = 0.25f + (0.5 / 2.0f - ypos / W_HEIGHT) * 2;
}
