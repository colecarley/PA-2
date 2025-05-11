#pragma once

#include "mesh.h"
#include <glm/glm.hpp>
#include <vector>

class AsteroidBelt {
public:
  AsteroidBelt(std::shared_ptr<Mesh> asteroidMesh, unsigned int count = 1000);
  void Initialize();
  void Render(GLint projectionLoc, GLint viewLoc, GLint useInstancingLoc,
              const glm::mat4 &projection, const glm::mat4 &view);

private:
  std::shared_ptr<Mesh> mesh;
  unsigned int instanceCount;
  GLuint instanceVBO;
  std::vector<glm::mat4> modelMatrices;
};
