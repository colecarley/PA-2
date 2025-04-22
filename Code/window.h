#pragma once

#include </opt/homebrew/opt/glew/include/GL/glew.h>
#include </opt/homebrew/opt/glfw/include/GLFW/glfw3.h>
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
