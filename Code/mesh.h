#pragma once

#include "object.h"
#include <cstdlib>
#include <string>

class Mesh : public Object {
private:
  const std::string &mesh_file_path;

public:
  GLuint getVAO() const { return this->vao; }
  GLsizei getIndexCount() const { return static_cast<GLsizei>(Indices.size()); }
  GLuint getTextureID() const { return this->texture->get_texture_id(); }

  Mesh(const std::string &mesh_file_path, const std::string& texture_file_path);
  void createVertices();
};
