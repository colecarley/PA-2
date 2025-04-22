#pragma once
#include "object.h"

class Pyramid : public Object {
public:
  Pyramid();

private:
  void createVertices() override;
};
