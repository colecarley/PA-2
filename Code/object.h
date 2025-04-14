#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"
#include <vector>

class Object {
public:
  Object();
  virtual ~Object() = 0;
  virtual void Initialize(GLint posAttribLoc, GLint colAttribLoc) = 0;
  virtual void Update(unsigned int dt) = 0;
  virtual void Render(GLint posAttribLoc, GLint colAttribLoc) = 0;
  virtual void createVertices() = 0;
  virtual glm::mat4 GetModel() = 0;

  // angle setter
  virtual void setAngle(float angle) = 0;
  virtual void setSpeed(glm::vec3 spd) = 0;

private:
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
