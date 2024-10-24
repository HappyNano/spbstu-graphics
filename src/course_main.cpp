#include <iostream>
#include <string>

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
#include "lab/text.hpp"
#include "lab/fps_counter.hpp"

#include "lab/particles/particle_system.hpp"
#include "lab/particles/anti_attractor.hpp"
#include "lab/particles/surface_attractor.hpp"
#include "lab/particles/cube_collider.hpp"
#include "lab/particles/point_particle_generator.hpp"
#include "lab/particles/cylindre_particle_generator.hpp"
#include "lab/particles/sphere_particle_generator.hpp"
#include "lab/particles/cylinder_attractor.hpp"
#include "lab/particles/cone_particle_generator.hpp"
#include "lab/particles/sphere_collider.hpp"

#include "threads/thread_pool.hpp"

void check(bool error, const std::string & msg, std::function< void(void) > todo = {}) noexcept(false)
{
  if (error)
  {
    todo();
    throw std::logic_error(msg);
  }
}

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

// meshes
unsigned int planeVAO;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting info
glm::vec3 lightPos(-5.0f, 4.0f, -2.0f);

void setupViewport(GLFWwindow * window);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);

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

  FT_Library ft;
  check(FT_Init_FreeType(&ft), "Could not init freetype library");

  Text text(ft, "assets/font.ttf");

  // Подключение шейдеров и текстур
  // ------------------------------
  Texture2D grass_texture = loadTexture("assets/grass.png");
  Shader shader("src/shaders/shading.vert", "src/shaders/shading.frag");
  Shader simpleDepthShader("src/shaders/depth.vert", "src/shaders/depth.frag");
  auto font_shader = Shader::makeShared("src/shaders/font.vert", "src/shaders/font.frag");
  auto particleShader = Shader::makeShared("src/shaders/particle_shader.vert", "src/shaders/particle_shader.frag");

  {
    glm::mat4 font_projection_m = glm::ortho(0.0f, static_cast< float >(W_WIDTH), 0.0f, static_cast< float >(W_HEIGHT));
    font_shader->use();
    font_shader->setMat4("projection", font_projection_m);
  }

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
  glEnable(GL_BLEND);

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
  auto surface = std::make_shared< Surface >();

  auto surface2 = std::make_shared< Surface >(1.0f);
  surface2->modelView().translate({ 0.0f, 5.0f, 0.0f });

  auto surface3 = std::make_shared< Surface >(1.0f);
  surface3->modelView().translate({ 0.0f, 2.0f, 0.0f });

  auto sphere = std::make_shared< Sphere >(0.5f);
  sphere->modelView().translate({ 3.0f, 2.5f, -1.5f });

  auto sphere_gen = std::make_shared< Sphere >(0.5f);
  sphere_gen->modelView().translate({ 0.0f, 3.5f, 0.0f });

  auto cube = std::make_shared< Cube >(1.0f);
  cube->modelView().translate({ 3.0f, 2.5f, 1.5f });

  auto cylinder = std::make_shared< Cylindre >(0.5f, 1.0f);
  cylinder->modelView().translate({ -2.0f, 3.0f, 0.0f });

  auto cone = std::make_shared< Cone >(0.5f, 1.0f);
  cone->modelView().translate({ 0.0f, 0.5f, 0.0f });

  auto render_scene = [&](Shader & shader)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grass_texture.ID);
    surface->render_modelView(shader);
    // cone->render_modelView(shader);
    surface2->render_modelView(shader);
    surface3->render_modelView(shader);
    cube->render_modelView(shader);
    sphere->render_modelView(shader);
    sphere_gen->render_modelView(shader);
    cylinder->render_modelView(shader);
  };

  // Particle System
  // ---------------
  // auto anti_attractor = AntiAttractor(glm::vec3(-2.0f, 1.5f, 0.0f), 0.5f);
  auto surface_attractor1 = SurfaceAttractor(std::dynamic_pointer_cast< Surface >(surface2), 2.0f);
  auto surface_attractor2 = SurfaceAttractor(std::dynamic_pointer_cast< Surface >(surface3), 2.0f);
  auto cube_collider = CubeCollider(std::dynamic_pointer_cast< Cube >(cube));
  auto sphere_collider = SphereCollider(std::dynamic_pointer_cast< Sphere >(sphere));
  auto cylinder_attractor = CylinderAttractor(std::dynamic_pointer_cast< Cylindre >(cylinder), -4.0f);
  // auto point_particle_gen = PointParticleGenerator(glm::vec3(0.0f, 0.5f, 0.0f));
  auto sphere_particle_gen = SphereParticleGenerator(std::dynamic_pointer_cast< Sphere >(sphere_gen));
  auto cone_particle_gen = ConeParticleGenerator(std::dynamic_pointer_cast< Cone >(cone));
  auto particles = ParticleSystem(particleShader, 5000,
   [&sphere_particle_gen]()
   {
     auto particle = sphere_particle_gen();
     particle.set_traceLength(3);
     float rColor = 0.5f + ((rand() % 100) / 100.0f);
     particle.color = glm::vec4(rColor, rColor, rColor, 1.0f); // Color
     particle.life = 5.0f * (1.0f + (rand() % 100) / 100.0f);  // Life
     return std::move(particle);
   });

  ThreadPool pool(1);
  std::future< void > particle_ready = pool.enqueue([&]() {});
  FpsCounter fps(glfwGetTime());
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

    shader.use();
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

    glCullFace(GL_FRONT);
    render_scene(simpleDepthShader);
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
    render_scene(shader);

    // Particle Scene
    particleShader->use();
    particleShader->setMat4("projection", projection);
    particleShader->setMat4("view", view);
    particle_ready.wait();
    particles.render();
    particle_ready = pool.enqueue(
     [&]()
     {
       particles.update(deltaTime, 50,
        [&](Particle & particle, float dt)
        {
          surface_attractor1(particle, dt);
          surface_attractor2(particle, dt);
          cylinder_attractor(particle, dt);
          cube_collider(particle, dt);
          sphere_collider(particle, dt);
          // Kill particle if particle below y=0.0
          if (particle.pos.y <= 0.0f || particle.pos.y >= 8.0f     //
              || particle.pos.x <= -5.0f || particle.pos.x >= 5.0f //
              || particle.pos.z <= -5.0f || particle.pos.z >= 5.0f)
          {
            particle.kill();
          }
        });
     });
    glUseProgram(0);
    // End Particle
    text.render(font_shader, std::to_string(fps.get()), 0.5f, W_HEIGHT - 20.0f, 0.5f, glm::vec3(0.2, 0.8f, 0.2f));
    text.render(font_shader, std::to_string(particles.aliveCount()), 0.5f, W_HEIGHT - 45.0f, 0.5f, glm::vec3(0.2, 0.8f, 0.2f));

    // Конец  отрисовки
    glFlush();
    glfwSwapBuffers(window);

    // Подсчет кадров
    fps.update(glfwGetTime());
  }
  glDeleteTextures(1, &grass_texture.ID);
  glDeleteTextures(1, &depthMap);

  glDeleteFramebuffers(1, &depthMapFBO);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
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
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(UP, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(DOWN, deltaTime);
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
  static float lastX = (float)W_WIDTH / 2.0;
  static float lastY = (float)W_HEIGHT / 2.0;
  static bool firstMouse = true;

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
