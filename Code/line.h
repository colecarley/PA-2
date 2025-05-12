#pragma once
#include "graphics_headers.h"
#include "shader.h"
#include <memory>
#include <vector>

class Line {
private:
  std::vector<glm::vec3> planet_trail;
  size_t max_trail = 1000;
  GLuint vao;
  GLuint VB;
  std::unique_ptr<Shader> shader;
  void initialize_shaders();

public:
  Line(size_t max_trail);
  ~Line();

  void add_position(glm::vec3);
  void initialize();
  void render(glm::mat4 projection, glm::mat4 view);
};
