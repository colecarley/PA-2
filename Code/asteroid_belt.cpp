// asteroid_belt.cpp
#include "asteroid_belt.h"
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

AsteroidBelt::AsteroidBelt(std::shared_ptr<Mesh> asteroidMesh,
    const BeltSettings& settings)
    : mesh(std::move(asteroidMesh)),
    cfg(settings),
    instanceCount(settings.count) {
}

void AsteroidBelt::Initialize() {

    modelMatrices.reserve(instanceCount);

    for (unsigned int i = 0; i < instanceCount; ++i) {
        glm::mat4 model(1.0f);
        float angle = static_cast<float>(i) / instanceCount * 360.0f;

        auto randf = [](float a, float b) {
            return a + static_cast<float>(rand()) / RAND_MAX * (b - a);
            };

        float x = std::sin(glm::radians(angle)) * cfg.radius
            + randf(-cfg.jitter, cfg.jitter);
        float y = randf(-cfg.vertFactor, cfg.vertFactor);
        float z = std::cos(glm::radians(angle)) * cfg.radius
            + randf(-cfg.jitter, cfg.jitter);

        model = glm::translate(model, { x, y, z });

        float scl = randf(cfg.minScale, cfg.maxScale);
        model = glm::scale(model, glm::vec3(scl));

        float rotAngle = randf(0.0f, 360.0f);
        model = glm::rotate(model, glm::radians(rotAngle),
            glm::vec3(0.4f, 0.6f, 0.8f));

        modelMatrices.push_back(model);
    }

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
        modelMatrices.size() * sizeof(glm::mat4),
        modelMatrices.data(),
        GL_STATIC_DRAW);

    GLuint vao = mesh->getVAO();
    glBindVertexArray(vao);            

    std::size_t vec4Size = sizeof(glm::vec4);
    for (int i = 0; i < 4; ++i) {
        GLuint loc = 3 + i; 
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE,
            sizeof(glm::mat4),
            reinterpret_cast<void*>(i * vec4Size));
        glVertexAttribDivisor(loc, 1);
    }

    glBindVertexArray(0);
}


void AsteroidBelt::Render(GLint projectionLoc, GLint viewLoc,
    GLint useInstancingLoc,
    const glm::mat4& projection,
    const glm::mat4& view)
{
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    glUniform1i(useInstancingLoc, 1);

    glBindVertexArray(mesh->getVAO());
    glDrawElementsInstanced(GL_TRIANGLES,
        mesh->getIndexCount(),
        GL_UNSIGNED_INT,
        nullptr,
        instanceCount);
    glBindVertexArray(0);

    glUniform1i(useInstancingLoc, 0);
}

