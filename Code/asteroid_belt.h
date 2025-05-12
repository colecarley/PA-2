#pragma once

#include "mesh.h"
#include "solar_constants.h"
#include <vector>
#include <glm/glm.hpp>

class AsteroidBelt {
public:
	AsteroidBelt(std::shared_ptr<Mesh> mesh, const BeltSettings& cfg);
	void Initialize();
	void Render(GLint projectionLoc, GLint viewLoc, GLint useInstancingLoc, const glm::mat4& projection, const glm::mat4& view);

private:
	BeltSettings cfg;
	std::shared_ptr<Mesh> mesh;
	unsigned int instanceCount;
	GLuint instanceVBO;
	std::vector<glm::mat4> modelMatrices;
};
