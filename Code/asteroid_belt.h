#pragma once

#include "mesh.h"
#include "shader_variable_locations.h"
#include "solar_constants.h"
#include <glm/glm.hpp>
#include <vector>

class AsteroidBelt {
public:
  AsteroidBelt(std::shared_ptr<Mesh> mesh, const Belt &settings);
  void Initialize();
  void Render(const ShaderVariableLocations &, const glm::mat4 &projection,
              const glm::mat4 &view);

private:
  Belt settings;
  std::shared_ptr<Mesh> mesh;
  unsigned int instanceCount;
  GLuint instanceVBO;
  std::vector<glm::mat4> modelMatrices;
};
