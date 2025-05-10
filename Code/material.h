#pragma once
#include "graphics_headers.h"

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

  Material()
      : ambient(0.1f, 0.1f, 0.1f), diffuse(1.0f, 0.8f, 0.7f),
        specular(0.5f, 0.4f, 0.3f), shininess(32) {}
  Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh)
      : ambient(a), diffuse(d), specular(s), shininess(sh) {}
};
