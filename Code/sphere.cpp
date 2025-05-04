#include "sphere.h"


Sphere::Sphere() { this->createVertices(); }

void Sphere::createVertices() {
  int prec = 10;
  for (int i = 0; i <= prec; i++) {
    for (int j = 0; j <= prec; j++) {
      float y = (float)cos(glm::radians(180.f - i * 180.f / prec));
      float x =
          -(float)cos(glm::radians(j * 360.f / prec)) * (float)(abs(cos(asin(y))));
      float z =
          (float)sin(glm::radians(j * 360.f / prec)) * (float)(abs(cos(asin(y))));

      auto vec = glm::vec3(x, y, z);
      this->Vertices.push_back(Vertex(vec, vec, glm::vec2(0)));
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
