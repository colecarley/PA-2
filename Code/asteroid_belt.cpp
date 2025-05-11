#include "asteroid_belt.h"
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

AsteroidBelt::AsteroidBelt(std::shared_ptr<Mesh> asteroidMesh,
                           unsigned int count)
    : mesh(asteroidMesh), instanceCount(count) {}

void AsteroidBelt::Initialize() {
  // std::cout << "[AsteroidBelt] Initialize() called\n";

  float radius = 6.0f;
  float offset = 1.5f;

  for (unsigned int i = 0; i < instanceCount; ++i) {
    glm::mat4 model(1.0f);
    float angle = (float)i / (float)instanceCount * 360.0f;

    float displacement = ((rand() % (int)(2 * offset * 100)) / 100.0f) - offset;
    float x = sin(glm::radians(angle)) * radius + displacement;
    displacement = ((rand() % (int)(2 * offset * 100)) / 100.0f) - offset;
    float y = displacement * 0.4f;
    displacement = ((rand() % (int)(2 * offset * 100)) / 100.0f) - offset;
    float z = cos(glm::radians(angle)) * radius + displacement;

    model = glm::translate(model, glm::vec3(x, y, z));
    float scale = ((rand() % 20) / 100.0f) + 0.05f;
    model = glm::scale(model, glm::vec3(scale));
    float rotAngle = (rand() % 360);
    model =
        glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.4f, 0.6f, 0.8f));

    modelMatrices.push_back(model);
  }

  // 1. Generate and bind the instance VBO
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4),
               &modelMatrices[0], GL_STATIC_DRAW);

  // 2. Bind the base mesh VAO AFTER the mesh has been initialized
  GLuint vao = mesh->getVAO();
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // Must bind here!

  std::size_t vec4Size = sizeof(glm::vec4);
  for (int i = 0; i < 4; ++i) {
    glEnableVertexAttribArray(3 + i);
    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(i * vec4Size));
    glVertexAttribDivisor(3 + i, 1);
  }

  glBindVertexArray(0);
}

void AsteroidBelt::Render(GLint projectionLoc, GLint viewLoc,
                          GLint useInstancingLoc, const glm::mat4 &projection,
                          const glm::mat4 &view) {
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

  glUniform1i(useInstancingLoc, 1); // Enable instancing

  GLuint vao = mesh->getVAO();
  glBindVertexArray(vao);

  // std::cout << "[AsteroidBelt] Rendering " << instanceCount
  //     << " instances with VAO " << vao
  //     << ", Index count = " << mesh->getIndexCount() << std::endl;

  glDrawElementsInstanced(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT,
                          0, instanceCount);

  glBindVertexArray(0);
  glUniform1i(useInstancingLoc, 0); // Reset

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cerr << "[AsteroidBelt] GL ERROR: " << err << std::endl;
  }
}
