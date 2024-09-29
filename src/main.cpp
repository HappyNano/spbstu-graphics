#include <iostream>
#include <functional>
#include <string>

// Подключение GLAD до GLFW
#include "glad/glad.h"
#include <GL/freeglut.h>
#include <glm/ext.hpp>

#include <GLFW/glfw3.h>

#include <math.h>
#include <unistd.h>

#include "lab1/settings.hpp"
#include "lab1/material.hpp"
#include "lab1/texture.hpp"
#include "lab1/shaders.hpp"
#include "lab1/camera.hpp"

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

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)W_WIDTH / 2.0;
float lastY = (float)W_HEIGHT / 2.0;
bool firstMouse = true;

// meshes
unsigned int planeVAO;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

void ConfigureShaderAndMatrices();
void renderScene(Shader & shader);

void displaySurface(Shader & shader);
void displayCylindre();
void displaySphere(Shader & shader);
void displayCube(Shader & shader);
void displayTor();

int main(int argc, char ** argv)
{
  glutInit(&argc, argv); // Инициализация GLUT

  check(!glfwInit(), "GLFW Bad initialization"); // Инициализация GLFW

  // Окно
  // ----
  GLFWwindow * window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Lab1", nullptr, nullptr);
  check(window == nullptr, "Failed to create GLFW window", []() { glfwTerminate(); });
  glfwMakeContextCurrent(window);

  // Инициализация GLAD
  // ------------------
  check(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLEW");

  // Настройка GLFW
  // --------------
  setupViewport(window);

  glfwSetKeyCallback(window, key_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  // Подключение шейдеров и текстур
  textureID = loadTexture("assets/wood.png");
  Shader shader("src/shaders/shading.vert", "src/shaders/shading.frag");
  Shader simpleDepthShader("src/shaders/depth.vert", "src/shaders/depth.frag");
  Shader debugDepthQuad("src/shaders/debug_quad.vert", "src/shaders/debug_quad.frag");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float planeVertices[] = {
    // positions            // normals         // texcoords
    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,   //
    -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   //
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f, //

    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,   //
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f, //
    25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f  //
  };
  // plane VAO
  unsigned int planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glBindVertexArray(0);

  // configure depth map FBO
  // -----------------------
  unsigned int depthMapFBO;
  glGenFramebuffers(1, &depthMapFBO);

  const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

  unsigned int depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // configuring
  // -----------
  glEnable(GL_DEPTH_TEST);

  // shader configuration
  // --------------------
  shader.use();
  shader.setInt("diffuseTexture", 0);
  shader.setInt("shadowMap", 1);

  debugDepthQuad.use();
  debugDepthQuad.setInt("depthMap", 0);

  // lighting info
  // -------------
  glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

  // Цикл отрисовки
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast< float >(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. сначала рисуем карту глубины
    // -------------------------------

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    simpleDepthShader.use();
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    renderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. рисуем сцену как обычно с тенями (используя карту глубины)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    setupViewport(window); // reset viewport
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    // set light uniforms
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightPos", lightPos);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderScene(shader);

    // render Depth map to quad for visual debugging
    // ---------------------------------------------
    debugDepthQuad.use();
    debugDepthQuad.setFloat("near_plane", near_plane);
    debugDepthQuad.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // Обновляем цвет формы
    // GLfloat timeValue = glfwGetTime();
    // GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
    // shader.setVec4("ourColor", glm::vec4{ 0.0f, greenValue, 0.0f, 1.0f });

    // Рисуем треугольник
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0);

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

void ConfigureShaderAndMatrices()
{
  // Set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 1000.0f);
  glTranslatef(0.0f, -0.25f, -2.0f);
}
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderScene(Shader & shader)
{
  // floor
  glm::mat4 model = glm::mat4(1.0f);
  shader.setMat4("model", model);
  glBindVertexArray(planeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  // displaySurface(shader);

  // displaySphere(shader);
  // displayTor();
  if (cubeVAO == 0)
  {
    float vertices[] = {
      // back face
      -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
      1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
      1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
      1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
      -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
      -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
      // front face
      -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
      1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
      1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
      1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
      -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
      -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
      // left face
      -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
      -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
      -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
      -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
      -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
      -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                          // right face
      1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
      1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
      1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
      1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
      1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
      1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
      // bottom face
      -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
      1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
      1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
      1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
      -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
      -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
      // top face
      -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
      1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
      1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
      1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
      -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
      -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // render Cube
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  // displayCube(shader);
  // displayCylindre();
}

void displaySurface(Shader & shader)
{
  glm::mat4 model = glm::mat4(1.0f);

  glPushAttrib(GL_LIGHTING_BIT);
  glPushMatrix();

  glEnable(GL_BLEND);
  setMaterial(MaterialConf{ { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.1f, 0.1f, 0.1f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 5.0f } });

  // glTranslatef(0.0f, 0.0f, -6.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f));
  shader.setMat4("model", model);
  // glRotatef(90, 0.0f, 0.0f, -3.0f);

  glBegin(GL_QUADS);
  GLfloat size = 4.0f;
  glVertex3f(-size, -size, size);
  glVertex3f(size, -size, size);
  glVertex3f(size, size, size);
  glVertex3f(-size, size, size);
  glEnd();

  glPopMatrix();
  glPopAttrib();
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

void displaySphere(Shader & shader)
{
  glm::mat4 model = glm::mat4(1.0f);

  glPushAttrib(GL_LIGHTING_BIT);
  glPushMatrix();

  setMaterial(MaterialConf{ { 0.0f, 0.7f, 0.3f, 1.0f }, { 0.1f, 0.1f, 0.1f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 5.0f } });
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.5f, 128, 128);

  glPopMatrix();
  glPopAttrib();
}

void displayCube(Shader & shader)
{
  glm::mat4 model = glm::mat4(1.0f);

  static GLfloat angle = 0.0f;
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  glPushMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // glColor4f(0.8f, 0.1f, 0.1f, 1.0f);

  // setMaterial(MaterialConf{ { 0.7f, 0.4f, 0.1f, 1.0f }, { 0.2f, 0.1f, 0.1f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f } });

  // glRotatef((sin(angle += 0.001) + 1) * 45.f, 1.0f, 0.0f, 0.0f);
  // glTranslatef(-1.0f, 0.0f, 0.0f);
  // glScalef(0.5f, 0.5f, 0.5f);

  model = glm::rotate(model, glm::radians((sin(angle += 0.001) + 1) * 45.f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
  model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
  shader.setMat4("model", model);

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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(static_cast< float >(yoffset));
}

void cursor_position_callback(GLFWwindow * window, double xposIn, double yposIn)
{
  float xpos = static_cast< float >(xposIn);
  float ypos = static_cast< float >(yposIn);
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
