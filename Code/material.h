#pragma once
#include "graphics_headers.h"

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

  Material() : ambient(0.1, 0.1, 0.1), diffuse(1, 1, 1), specular(1, 1, 1), shininess(32) {}
  Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh)
      : ambient(a), diffuse(d), specular(s), shininess(sh) {}
};
