#include <iostream>
#include <functional>
#include <string>
#include <memory>

// Подключение GLAD до GLFW
#include "glad/glad.h"
#include <GL/freeglut.h>
#include <glm/ext.hpp>

#include <GLFW/glfw3.h>

#include <math.h>
#include <unistd.h>

#include "lab/settings.hpp"
#include "lab/material.hpp"
#include "lab/texture.hpp"
#include "lab/shaders.hpp"
#include "lab/camera.hpp"

#include "lab/figures/surface.hpp"
#include "lab/figures/cube.hpp"
#include "lab/figures/cylindre.hpp"
#include "lab/figures/torus.hpp"
#include "lab/figures/sphere.hpp"

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

Texture2D texture, glass_texture, grass_texture, metal_texture;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = (float)W_WIDTH / 2.0;
float lastY = (float)W_HEIGHT / 2.0;
bool firstMouse = true;
bool lclick = false;

// meshes
unsigned int planeVAO;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting info
glm::vec3 lightPos(-5.0f, 4.0f, -2.0f);

// figures
std::unique_ptr< Figure > surface;
std::unique_ptr< Figure > cube;
std::unique_ptr< Figure > cylindre;
std::unique_ptr< Figure > torus;
std::unique_ptr< Figure > sphere;

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
void renderScene(Shader & shader, bool render_scene = true);

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Подключение шейдеров и текстур
  texture = loadTexture("assets/wood-2.png");
  glass_texture = loadTexture("assets/glass1.png");
  grass_texture = loadTexture("assets/grass.png");
  metal_texture = loadTexture("assets/metal.png");
  Shader shader("src/shaders/shading.vert", "src/shaders/shading.frag");
  Shader simpleDepthShader("src/shaders/depth.vert", "src/shaders/depth.frag");

  // Создаем и настраиваем карту глубины FBO
  // -----------------------
  unsigned int depthMapFBO, depthMap;
  glGenFramebuffers(1, &depthMapFBO);

  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

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
  shader.setInt("shadowMap", depthMapFBO);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  shader.setVec3("lightColor", glm::vec3(0.6));
  shader.setFloat("alpha", 1.0f);

  shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  shader.setVec3("material.ambient", 0.6f, 0.6f, 0.6f);
  shader.setVec3("material.diffuse", 0.7f, 0.7f, 0.7f);
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setFloat("material.shininess", 16.0f);

  // Figures creating
  // ----------------
  surface = std::make_unique< Surface >();
  cube = std::make_unique< Cube >(2.0f);
  cylindre = std::make_unique< Cylindre >(0.5f, 3.0f);
  torus = std::make_unique< Torus >(0.5f, 1.0f);
  sphere = std::make_unique< Sphere >(0.5f);

  // Цикл отрисовки
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    glfwPollEvents();

    float currentFrame = static_cast< float >(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Цвет фона
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Изменение света
    // ---------------

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 19.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    shader.setVec3("lightPos", lightPos); // Менять параметры при изменении положения света
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix); // -- || --

    // 1. сначала рисуем карту глубины
    // -------------------------------
    simpleDepthShader.use();
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.ID); // ??

    glCullFace(GL_FRONT);
    renderScene(simpleDepthShader, false);
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    setupViewport(window); // reset viewport

    // 2. рисуем сцену как обычно с тенями (используя карту глубины)
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    // set light uniforms
    shader.setVec3("viewPos", camera.Position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderScene(shader);

    glFlush();
    // == Конец  отрисовки
    glfwSwapBuffers(window);
  }
  glDeleteTextures(1, &texture.ID);
  glDeleteTextures(1, &depthMap);

  glDeleteFramebuffers(1, &depthMapFBO);

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

void renderScene(Shader & shader, bool render_scene)
{
  static float angle = 0.0f;
  // floor
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, grass_texture.ID);
  glm::mat4 model = glm::mat4(1.0f);
  shader.setMat4("model", model);
  if (render_scene)
  {
    shader.setVec3("material.ambient", 0.3f, 0.6f, 0.3f);
    shader.setVec3("material.diffuse", 0.3f, 0.6f, 0.3f);
    shader.setVec3("material.specular", 0.3f, 0.5f, 0.3f);
    shader.setFloat("material.shininess", 5.0f);
    shader.setFloat("alpha", 0.3f);
  }
  surface->render();
  shader.setVec3("material.ambient", 209.f / 255.f, 185.f / 255.f, 151.f / 255.f);
  shader.setVec3("material.diffuse", 209.f / 255.f, 185.f / 255.f, 151.f / 255.f);
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setFloat("material.shininess", 16.0f);
  shader.setFloat("alpha", 1.0f);

  // cube
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, metal_texture.ID);
  model = glm::mat4(1.0f);
  shader.setMat4("model", model);
  shader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
  shader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setFloat("material.shininess", 90.0f);
  shader.setFloat("alpha", 1.0f);
  cube->render();

  // cylindre
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.ID);
  shader.setVec3("material.ambient", 209.f / 255.f, 185.f / 255.f, 151.f / 255.f);
  shader.setVec3("material.diffuse", 209.f / 255.f, 185.f / 255.f, 151.f / 255.f);
  shader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
  shader.setFloat("material.shininess", 8.0f);
  shader.setFloat("alpha", 1.0f);

  model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 2.0f));
  shader.setMat4("model", model);
  cylindre->render();
  model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 3.0f, 2.0f));
  shader.setMat4("model", model);
  sphere->render();
  model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 1.0f));
  shader.setMat4("model", model);
  cylindre->render();
  model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 3.0f, 1.0f));
  shader.setMat4("model", model);
  sphere->render();
  model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.0f, 2.0f));
  model = glm::rotate(model, glm::radians(-18.5f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
  model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
  shader.setMat4("model", model);
  cylindre->render();

  shader.setVec3("material.ambient", 209.f / 255.f, 185.f / 255.f, 151.f / 255.f);
  shader.setVec3("material.diffuse", 209.f / 255.f, 185.f / 255.f, 151.f / 255.f);
  shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.setFloat("material.shininess", 16.0f);
  shader.setFloat("alpha", 1.0f);

  // torus
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-3.7f, 1.5f, -2.0f));
  model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
  model = glm::rotate(model, glm::radians(25.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
  model = glm::rotate(model, glm::radians(angle += 0.01), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
  shader.setMat4("model", model);
  torus->render();

  // sphere
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, glass_texture.ID);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-1.0f, 3.0f, -1.0f));
  shader.setMat4("model", model);
  if (render_scene)
  {
    shader.setVec3("material.ambient", 0.6f, 0.6f, 0.6f);
    shader.setVec3("material.diffuse", 0.6f, 0.6f, 0.6f);
    shader.setVec3("material.specular", 0.6f, 0.6f, 0.6f);
    shader.setFloat("material.shininess", 70.0f);
    shader.setFloat("alpha", 0.3f);
  }
  sphere->render();
  shader.setFloat("alpha", 1.0f);
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
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    lightPos.x -= 0.1;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    lightPos.x += 0.1;
  }
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(static_cast< float >(yoffset));
}

void cursor_position_callback(GLFWwindow * window, double xposIn, double yposIn)
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
  {
    return;
  }
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
