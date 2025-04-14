#include "pyramid.h"

Pyramid::Pyramid() { this->createVertices(); }

void Pyramid::createVertices() {
  this->Vertices = {{{1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
                    {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
                    {{-1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                    {{-1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
                    {{0.0f, 2.0f, 0.0f}, {1.0f, 0.0f, 1.0f}}};

  this->Indices = {0, 1, 2, 2, 3, 0, 0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4};
}