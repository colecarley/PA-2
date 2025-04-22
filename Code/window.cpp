#include "window.h"
#include <memory>

using namespace std;

Window::Window(const char *name, int *width, int *height) {
  gWindow = NULL;

  if (!glfwInit()) {
    const char *err;
    glfwGetError(&err);
    printf("GLFW failed to initialize: %s\n", err);
    gWindow = NULL;
  }

  // Start OpenGL for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  gWindow = std::unique_ptr<GLFWwindow, DestroyglfwWin>(
      glfwCreateWindow(*width, *height, name, NULL, NULL));

  glfwMakeContextCurrent(gWindow.get());

  this->Initialize();
}

Window::~Window() {

  glfwDestroyWindow(gWindow.get());
  gWindow = NULL;
  glfwTerminate();
}

bool Window::Initialize() {
  // Start SDL
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cout << "exiting" << std::endl;
    std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;

    exit(EXIT_FAILURE);
  }
  glfwSwapInterval(1);

  // Any other Window Initialization goes here

  return true;
}

void Window::Swap() { glfwSwapBuffers(gWindow.get()); }
