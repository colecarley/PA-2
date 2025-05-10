#pragma once
#include "object.h"
#include "shader.h"
#include <array>
#include <memory>
#include <string>

class Skybox {
private:
  GLuint texture_id;
  std::array<glm::vec3, 8> vertices;
  std::array<unsigned int, 36> indices;
  GLuint vao;
  GLuint VB;
  GLuint IB;
  std::unique_ptr<Shader> shader;

public:
  Skybox(const std::array<std::string, 6> &cubemap_path);
  std::unique_ptr<Shader> &get_shader();
  void render();

private:
  void create_vertices();
  void add_textures(const std::array<std::string, 6> &cubemap_path);
  void initialize_shaders();
  void initialize();
};
