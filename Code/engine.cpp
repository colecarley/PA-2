
#include "engine.h"

Engine::Engine(const char *name, int width, int height) {
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
}

Engine::~Engine() {
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize() {
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if (!m_window->Initialize()) {

    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT)) {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // No errors
  return true;
}

void Engine::Run() {
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow())) {
    ProcessInput();
    Display(m_window->getWindow(), glfwGetTime());
    glfwPollEvents();
  }
  m_running = false;
}

void Engine::ProcessInput() {
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(m_window->getWindow(), true);

  // set angle and speed to 0 when keys are release
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_RELEASE &&
      glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_RELEASE &&
      glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT) == GLFW_RELEASE &&
      glfwGetKey(m_window->getWindow(), GLFW_KEY_RIGHT) == GLFW_RELEASE &&
      glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_RELEASE &&
      glfwGetKey(m_window->getWindow(), GLFW_KEY_O) == GLFW_RELEASE) {
    m_graphics->getObj()->setSpeed(glm::vec3(0., 0., 0.));
    m_graphics->getObj()->setAngle(0);
  }
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
    // move object up on key press
    m_graphics->getObj()->setSpeed(glm::vec3(0., .1, 0.));
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
    // move object down on key press
    m_graphics->getObj()->setSpeed(glm::vec3(0., -.1, 0.));
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    // move object right on key press
    m_graphics->getObj()->setSpeed(glm::vec3(.1, 0., 0.));
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
    // move object left on key press
    m_graphics->getObj()->setSpeed(glm::vec3(-.1, 0., 0.));
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) {
    // set angle clockwise
    m_graphics->getObj()->setAngle(-3.14159f * 0.01);
  }
  if (glfwGetKey(m_window->getWindow(), GLFW_KEY_O) == GLFW_PRESS) {
    // set angle counter clockwise
    m_graphics->getObj()->setAngle(3.14159f * 0.01);
  }
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

void Engine::Display(GLFWwindow *window, double time) {
  // render the objects
  m_graphics->Render();

  // swap the buffers
  m_window->Swap();

  // update the graphics
  m_graphics->Update(time, speed);
}
