#include "engine.h"
#include "graphics_headers.h"
#include "mode.h"
#include "window.h"
#include <memory>

void Engine::on_scroll(GLFWwindow *window, double xoffset, double yoffset) {
  float fov_delta = (float)yoffset * 3.f;

  Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
  engine->m_graphics->getCamera()->update_perspective(
      fov_delta, engine->m_WINDOW_WIDTH, engine->m_WINDOW_HEIGHT);
}

void Engine::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
  Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

  float offset_x = 0;
  float offset_y = 0;

  if (!engine->first_mouse) {
    offset_x = xpos - engine->last_x;
    offset_y = ypos - engine->last_y;
    engine->last_x = xpos;
    engine->last_y = ypos;
  }

  engine->first_mouse = false;

  float sensitivity = 0.1f;
  offset_x *= sensitivity;
  offset_y *= sensitivity;

  engine->yaw += offset_x;
  engine->pitch += offset_y;

  if (engine->pitch > 89.0f) {
    engine->pitch = 89.0f;
  } else if (engine->pitch < -89.0f) {
    engine->pitch = -89.0f;
  }
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
  glfwSetInputMode(m_window->getWindow().get(), GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED);

  glfwSetWindowUserPointer(this->m_window->getWindow().get(), this);
  glfwSetScrollCallback(this->m_window->getWindow().get(), this->on_scroll);
  glfwSetCursorPosCallback(this->m_window->getWindow().get(),
                           this->on_mouse_move);

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
  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window->getWindow().get(), true);
  }

  std::unique_ptr<Camera> &camera = this->m_graphics->getCamera();
  float speed = 2;
  glm::vec3 &camera_front = camera->get_front();
  glm::vec3 &camera_up = camera->get_up();
  glm::vec3 norm_cross = glm::normalize(glm::cross(camera_front, camera_up));
  glm::vec3 delta = {0, 0, 0};

  if (this->mode == EXPLORATION) {
    const float dt = getDt();
    int w_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_W);
    int s_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_S);
    int a_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_A);
    int d_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_D);

    if (w_key == GLFW_PRESS) {
      delta += speed * camera_front * dt;
    }
    if (s_key == GLFW_PRESS) {
      delta -= speed * camera_front * dt;
    }
    if (a_key == GLFW_PRESS) {
      delta -= speed * norm_cross * dt;
    }
    if (d_key == GLFW_PRESS) {
      delta += speed * norm_cross * dt;
    }
  }

  if (this->mode == PLANETARY_OBSERVATION) {
    int left_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_LEFT);
    int right_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_RIGHT);

    if (left_key == GLFW_PRESS &&
        pressed_keys.find(GLFW_KEY_LEFT) == pressed_keys.end()) {
      this->pressed_keys.insert(GLFW_KEY_LEFT);
      this->focused_planet =
          (Planet)(((int)this->focused_planet - 1) % num_planets);
      if (this->focused_planet < 0) {
        this->focused_planet =
            (Planet)((int)this->focused_planet + num_planets);
      }
    }
    if (right_key == GLFW_PRESS &&
        pressed_keys.find(GLFW_KEY_RIGHT) == pressed_keys.end()) {
      this->pressed_keys.insert(GLFW_KEY_RIGHT);
      this->focused_planet =
          (Planet)(((int)this->focused_planet + 1) % num_planets);
    }

    if (left_key == GLFW_RELEASE) {
      pressed_keys.erase(GLFW_KEY_LEFT);
    }
    if (right_key == GLFW_RELEASE) {
      pressed_keys.erase(GLFW_KEY_RIGHT);
    }
  }

  this->m_graphics->getCamera()->update_look_at(delta, pitch, yaw);
  int enter_key = glfwGetKey(m_window->getWindow().get(), GLFW_KEY_ENTER);

  if (enter_key == GLFW_PRESS &&
      pressed_keys.find(GLFW_KEY_ENTER) == pressed_keys.end()) {
    pressed_keys.insert(GLFW_KEY_ENTER);
    this->switch_modes();
  }

  if (enter_key == GLFW_RELEASE) {
    pressed_keys.erase(GLFW_KEY_ENTER);
  }
}

void Engine::Display(std::unique_ptr<GLFWwindow, DestroyglfwWin> &window,
                     double time) {
  // render the objects
  m_graphics->Render(mode);

  // swap the buffers
  m_window->Swap();

  // update the graphics
  m_graphics->Update(time, mode, focused_planet);
}
float Engine::getDt() {
  double now = glfwGetTime();
  float dt = now - this->last_time;
  this->last_time = now;
  return dt;
}

void Engine::switch_modes() {
  if (this->mode == PLANETARY_OBSERVATION) {
    this->mode = EXPLORATION;
  } else {
    this->mode = PLANETARY_OBSERVATION;
  }
}
