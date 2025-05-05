#pragma once
#include "graphics_headers.h"
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
              GLint samplerAttribLoc);
  glm::mat4 GetModel();

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
};
