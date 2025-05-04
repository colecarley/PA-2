#pragma once

#include "object.h"
#include <cstdlib>
#include <string>

class Mesh : public Object {
private:
  const std::string &mesh_file_path;

public:
  Mesh(const std::string &mesh_file_path, const std::string& texture_file_path);
  void createVertices();
};
