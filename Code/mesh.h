#pragma once

#include "graphics_headers.h"
#include "object.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Mesh : public Object {
private:
  const std::string &file_path;

public:
  Mesh(const std::string &file_path);
  void createVertices();
};
