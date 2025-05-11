#pragma once
#include "graphics_headers.h"
#include "material.h"
#include "shader_variable_locations.h"
#include "texture.h"
#include <memory>
#include <vector>

class Object {
public:
  Object() = default;
  virtual ~Object();
  void Initialize(const ShaderVariableLocations &);
  void Update(glm::mat4 model_matrix);
  void Render(const ShaderVariableLocations &, bool is_emissive = false);
  glm::mat4 GetModel();
  void set_material(Material m);

protected:
  virtual void createVertices() = 0;
  glm::mat4 model;
  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;
  GLuint vao;
  GLuint VB;
  GLuint IB;
  glm::vec3 m_speed = glm::vec3(0., 0., 0.);
  float angle;
  std::unique_ptr<Texture> texture;
  std::unique_ptr<Texture> normal_texture;
  bool has_normal_map = false;
  Material material;
  bool is_emissive = false;
};
