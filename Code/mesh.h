#pragma once

#include "object.h"
#include <cstdlib>
#include <string>

class Mesh : public Object {
private:
  const std::string &file_path;

public:
  Mesh(const std::string &file_path);
  void createVertices();
};
