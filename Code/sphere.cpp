#include "sphere.h"
#include "texture.h"

Sphere::Sphere(const std::string &texture_filename,
               const std::string &normal_texture_filename) {
  this->texture = std::make_unique<Texture>(texture_filename);
  this->normal_texture = std::make_unique<Texture>(normal_texture_filename);
	this->has_normal_map = true;
  this->createVertices();
}

Sphere::Sphere(const std::string &texture_filename) {
  this->texture = std::make_unique<Texture>(texture_filename);
  this->createVertices();
}

void Sphere::createVertices() {
  int prec = 25;
  for (int i = 0; i <= prec; i++) {
    for (int j = 0; j <= prec; j++) {
      float y = (float)cos(glm::radians(180.f - i * 180.f / prec));
      float x = -(float)cos(glm::radians(j * 360.f / prec)) *
                (float)(abs(cos(asin(y))));
      float z = (float)sin(glm::radians(j * 360.f / prec)) *
                (float)(abs(cos(asin(y))));

      auto vec = glm::vec3(x, y, z);
      this->Vertices.push_back(
          Vertex(vec, vec, glm::vec2((float)j / prec, (float)i / prec)));
    }
  }

  for (int i = 0; i < prec; i++) {
    for (int j = 0; j < prec; j++) {
      this->Indices.push_back(i * (prec + 1) + j);
      this->Indices.push_back(i * (prec + 1) + j + 1);
      this->Indices.push_back((i + 1) * (prec + 1) + j);
      this->Indices.push_back(i * (prec + 1) + j + 1);
      this->Indices.push_back((i + 1) * (prec + 1) + j + 1);
      this->Indices.push_back((i + 1) * (prec + 1) + j);
    }
  }
}
