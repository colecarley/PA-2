#ifndef WINDOW_H
#define WINDOW_H

#include </opt/homebrew/opt/glew/include/GL/glew.h>
#include </opt/homebrew/opt/glfw/include/GLFW/glfw3.h>
#include <iostream>
#include <string>

class Window {
public:
  Window(const char *name, int *width, int *height);
  ~Window();
  bool Initialize();
  void Swap();

  GLFWwindow *getWindow() { return gWindow; };

private:
  GLFWwindow *gWindow;
};

#endif /* WINDOW_H */
