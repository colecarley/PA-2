#include "engine.h"
#include "window.h"
#include <memory>

void Engine::on_scroll(GLFWwindow *window, double xoffset, double yoffset) {
  float fov_delta = (float)yoffset;

  std::cout << "yoffset" << yoffset << " fov_delta:" << fov_delta << std::endl;
  Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
  engine->m_graphics->getCamera()->update_perspective(
      fov_delta, engine->m_WINDOW_WIDTH, engine->m_WINDOW_HEIGHT);
}

Engine::Engine(const char *name, int width, int height) {
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  this->last_x = width / 2.0;
  this->last_y = height / 2.0;
}

Engine::~Engine() {}

bool Engine::Initialize() {
  // Start a window
  m_window = std::make_unique<Window>(m_WINDOW_NAME, &m_WINDOW_WIDTH,
                                      &m_WINDOW_HEIGHT);
  if (!m_window->Initialize()) {
    printf("The window failed to initialize.\n");
    return false;
  }

  glfwSetWindowUserPointer(this->m_window->getWindow().get(), this);
  glfwSetScrollCallback(this->m_window->getWindow().get(), this->on_scroll);

  // Start the graphics
  m_graphics = std::make_unique<Graphics>();
  if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT)) {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // No errors
  return true;
}

void Engine::Run() {
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow().get())) {
    ProcessInput();
    Display(m_window->getWindow(), glfwGetTime());
    glfwPollEvents();
  }
  m_running = false;
}

void Engine::ProcessInput() {
  double xpos, ypos;
  glfwGetCursorPos(this->m_window->getWindow().get(), &xpos, &ypos);

  float offset_x = 0;
  float offset_y = 0;

  if (!first_mouse) {
    offset_x = xpos - last_x;
    offset_y = ypos - last_y;
    this->last_x = xpos;
    this->last_y = ypos;
  }

  this->first_mouse = false;

  float sensitivity = 0.1f;
  offset_x *= sensitivity;
  offset_y *= sensitivity;

  this->yaw += offset_x;
  this->pitch += offset_y;

  if (this->pitch > 89.0f) {
    this->pitch = 89.0f;
  } else if (this->pitch < -89.0f) {
    this->pitch = -89.0f;
  }

  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window->getWindow().get(), true);
  }

  std::unique_ptr<Camera> &camera = this->m_graphics->getCamera();
  float speed = 5;
  glm::vec3 &camera_front = camera->get_camera_front();
  glm::vec3 &camera_up = camera->get_camera_up();
  glm::vec3 norm_cross = glm::normalize(glm::cross(camera_front, camera_up));

  glm::vec3 delta = {0, 0, 0};
  const float dt = getDt();

  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_W) == GLFW_PRESS) {
    delta += speed * camera_front * dt;
  }
  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_S) == GLFW_PRESS) {
    delta -= speed * camera_front * dt;
  }
  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_A) == GLFW_PRESS) {
    delta -= speed * norm_cross * dt;
  }
  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_D) == GLFW_PRESS) {
    delta += speed * norm_cross * dt;
  }

  this->m_graphics->getCamera()->update_look_at(delta, pitch, yaw);
}

void Engine::Display(std::unique_ptr<GLFWwindow, DestroyglfwWin> &window,
                     double time) {
  // render the objects
  m_graphics->Render();

  // swap the buffers
  m_window->Swap();

  // update the graphics
  m_graphics->Update(time);
}

float Engine::getDt() {
  double now = glfwGetTime();
  float dt = now - this->last_time;
  this->last_time = now;
  return dt;
}
