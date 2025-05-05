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
  void Display(std::unique_ptr<GLFWwindow, DestroyglfwWin> &, double);
  static void on_scroll(GLFWwindow *window, double xoffset, double yoffset);

private:
  // Window related variables

  std::unique_ptr<Window> m_window;
  const char *m_WINDOW_NAME;
  int m_WINDOW_WIDTH;
  int m_WINDOW_HEIGHT;
  bool m_FULLSCREEN;
  float last_x;
  float last_y;
  float yaw = 0;
  bool first_mouse = true;
  float pitch = M_PI;

  std::unique_ptr<Graphics> m_graphics;

  bool m_running;
};
