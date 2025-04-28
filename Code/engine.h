#pragma once

#include "graphics.h"
#include "window.h"
#include <assert.h>
#include <sys/time.h>

/**
 * graphics engine used by the main function
 */
class Engine {
public:
  Engine(const char *name, int width, int height);

  ~Engine();
  bool Initialize();
  void Run();
  void ProcessInput();
  unsigned int getDT();
  long long GetCurrentTimeMillis();
  void Display(std::unique_ptr<GLFWwindow, DestroyglfwWin> &, double);
  void setSpeed(glm::vec3 spd) { speed = spd; };

private:
  // Window related variables
  int last_x = 0;
  int last_y = 0;
  float yaw = 0;
  float pitch = 0;
  std::unique_ptr<Window> m_window;
  const char *m_WINDOW_NAME;
  int m_WINDOW_WIDTH;
  int m_WINDOW_HEIGHT;
  bool m_FULLSCREEN;

  glm::vec3 speed = glm::vec3(0.f, 0.f, 0.f);

  std::unique_ptr<Graphics> m_graphics;

  bool m_running;
};
