#pragma once

#include "object.h"

class Sphere : public Object {
public:
  Sphere(const std::string texture_filename);

private:
  void createVertices() override;
};
