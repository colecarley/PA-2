#pragma once

#include "graphics.h"
#include "mode.h"
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
  void Display(std::unique_ptr<GLFWwindow, DestroyglfwWin> &, double);
  static void on_scroll(GLFWwindow *window, double xoffset, double yoffset);
  static void on_mouse_move(GLFWwindow *window, double xoffset, double yoffset);
  float getDt();
  void switch_modes();

private:
  // Window related variables

  std::unique_ptr<Window> m_window;
  const char *m_WINDOW_NAME;
  int m_WINDOW_WIDTH;
  int m_WINDOW_HEIGHT;
  bool m_FULLSCREEN;
  float last_x;
  float last_y;
  float last_time = 0;
  float yaw = 0;
  float pitch = M_PI;
  bool first_mouse = true;

  std::unique_ptr<Graphics> m_graphics;
  Mode mode = PLANETARY_OBSERVATION;

  bool m_running;
};
