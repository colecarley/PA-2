
#include "engine.h"
#include "window.h"
#include <memory>

Engine::Engine(const char *name, int width, int height) {
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
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
  if (glfwGetKey(m_window->getWindow().get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(m_window->getWindow().get(), true);
}

unsigned int Engine::getDT() {
  // long long TimeNowMillis = GetCurrentTimeMillis();
  // assert(TimeNowMillis >= m_currentTimeMillis);
  // unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis -
  // m_currentTimeMillis); m_currentTimeMillis = TimeNowMillis; return
  // DeltaTimeMillis;
  return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis() {
  // timeval t;
  // gettimeofday(&t, NULL);
  // long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  // return ret;
  return (long long)glfwGetTime();
}

void Engine::Display(std::unique_ptr<GLFWwindow, DestroyglfwWin> &window,
                     double time) {
  // render the objects
  m_graphics->Render();

  // swap the buffers
  m_window->Swap();

  // update the graphics
  m_graphics->Update(time, speed);
}
