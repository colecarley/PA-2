#ifndef CUBE_H
#define CUBE_H

#include "graphics_headers.h"
#include "object.h"
#include <cstdlib>
#include <ctime>
#include <vector>

class Cube : public Object {
public:
  Cube();
  ~Cube();
  void Initialize(GLint posAttribLoc, GLint colAttribLoc) override;
  void Update(unsigned int dt) override;
  void Render(GLint posAttribLoc, GLint colAttribLoc) override;
  virtual void createVertices() override;
  glm::mat4 GetModel() override;

  void setAngle(float angle) override { this->angle = angle; };
  void setSpeed(glm::vec3 spd) override { this->m_speed = spd; };

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

#endif /* CUBE_H */
