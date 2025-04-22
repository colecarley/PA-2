#pragma once

#include "object.h"

class Cube : public Object {
public:
  Cube();

private:
  void createVertices() override;
};