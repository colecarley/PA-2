#pragma once

#include "object.h"

class Sphere : public Object {
public:
  Sphere();

private:
  void createVertices() override;
};
