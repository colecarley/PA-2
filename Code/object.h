#pragma once
#include "graphics_headers.h"
#include "material.h"
#include "texture.h"
#include <memory>
#include <vector>

class Object {
public:
  Object() = default;
  virtual ~Object();
  void Initialize(GLint posAttribLoc, GLint vertAttribLoc, GLint textAttribLoc);
  void Update(glm::mat4 model_matrix);
  void Render(GLint posAttribLoc, GLint vertAttribLoc, GLint textAttribLoc,
              GLint samplerAttribLoc, GLint sampler2AttribLoc,
              GLint light_pos_loc, GLint light_color_loc,
              GLint has_normal_map_loc, GLint material_ambient_loc,
              GLint material_specular_loc, GLint material_diffuse_loc,
              GLint material_shininess_loc, GLint is_emissive_loc,
              bool is_emissive);
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
