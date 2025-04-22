#pragma once

#include "object.h"

class Sphere : public Object {
private:
  glm::vec3 color;
  bool valid_color = false;

public:
  Sphere();
  Sphere(glm::vec3 color);

private:
  void createVertices() override;
};
