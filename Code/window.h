#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

struct DestroyglfwWin {
  void operator()(GLFWwindow *ptr) { glfwDestroyWindow(ptr); }
};

class Window {
public:
  Window(const char *name, int *width, int *height);
  ~Window();
  bool Initialize();
  void Swap();

  std::unique_ptr<GLFWwindow, DestroyglfwWin> &getWindow() { return gWindow; };

private:
  std::unique_ptr<GLFWwindow, DestroyglfwWin> gWindow;
};
