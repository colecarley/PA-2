#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"
#include <vector>

class Object {
public:
  Object() = default;
  ~Object();
  void Initialize(GLint posAttribLoc, GLint colAttribLoc);
  void Update(glm::mat4 model_matrix);
  void Render(GLint posAttribLoc, GLint colAttribLoc);
  glm::mat4 GetModel();
  void setSpeed(glm::vec3 spd) { m_speed = spd; }
  void setAngle(float theta) { angle = theta; }

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
};

#endif /* OBJECT_H */
