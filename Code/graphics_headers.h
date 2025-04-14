#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL

#include </opt/homebrew/opt/glew/include/GL/glew.h>
#include </opt/homebrew/opt/glfw/include/GLFW/glfw3.h>
#include <OpenGL/glu.h>

// GLM for matricies
#define GLM_FORCE_RADIANS
#include </opt/homebrew/opt/glm/include/glm/glm.hpp>
#include </opt/homebrew/opt/glm/include/glm/gtc/matrix_transform.hpp>
#include </opt/homebrew/opt/glm/include/glm/gtc/random.hpp>
#include </opt/homebrew/opt/glm/include/glm/gtc/type_ptr.hpp>
#include </opt/homebrew/opt/glm/include/glm/gtx/rotate_vector.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Vertex {
  glm::vec3 vertex;
  glm::vec3 color;

  Vertex(glm::vec3 v, glm::vec3 c) : vertex(v), color(c) {}
};

#endif /* GRAPHICS_HEADERS_H */
