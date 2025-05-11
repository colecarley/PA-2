#include "ring.h"
#include "texture.h"

Ring::Ring(const std::string& texture_file, const std::string& normal_map_file) {
    this->texture = std::make_unique<Texture>(texture_file);
    this->normal_texture = std::make_unique<Texture>(normal_map_file);
    this->has_normal_map = true;
    createVertices();
}

Ring::Ring(const std::string& texture_file) {
    this->texture = std::make_unique<Texture>(texture_file);
    this->has_normal_map = false;
    createVertices();
}

void Ring::createVertices() {
    const int segments = 100;
    const float inner_radius = 1.2f;
    const float outer_radius = 2.0f;

    for (int i = 0; i <= segments; ++i) {
        float angle = glm::two_pi<float>() * i / segments;
        float x = cos(angle);
        float z = sin(angle);

        glm::vec3 normal = glm::vec3(0, 1, 0);

        float u = i / (float)segments;

        this->Vertices.emplace_back(
            glm::vec3(x * outer_radius, 0, z * outer_radius),
            normal,
            glm::vec2(u, 1.0f)
        );

        this->Vertices.emplace_back(
            glm::vec3(x * inner_radius, 0, z * inner_radius),
            normal,
            glm::vec2(u, 0.0f)
        );
    }

    for (int i = 0; i < segments * 2; i += 2) {
        this->Indices.push_back(i);
        this->Indices.push_back(i + 1);
        this->Indices.push_back(i + 2);

        this->Indices.push_back(i + 1);
        this->Indices.push_back(i + 3);
        this->Indices.push_back(i + 2);
    }
}