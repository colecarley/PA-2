#pragma once

#include <iostream>
#include <memory>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h> // <GL/glu.h> for windows (note)

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff
// For windows
#ifndef M_PI
#define M_PI 3.1415926535
#endif // M_PI

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture;

  Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)
      : position(p), normal(n), texture(t) {}
};