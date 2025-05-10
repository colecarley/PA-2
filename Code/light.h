#pragma once
#include "graphics_headers.h"

class Light {
  glm::vec4 global_ambient;
  glm::vec4 light_ambient;
  glm::vec4 light_diffuse;
  glm::vec4 light_specular;

  glm::vec3 light_position_view_space;
  glm::vec3 light_position;

public:
  Light(glm::vec4 viewMatrix);
};
