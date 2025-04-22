#include "mesh.h"

void Mesh::createVertices() {
  float x, y, z;
  std::string content;
  std::ifstream file_stream(file_path, std::ios::in);
  if (!file_stream) {
    std::cerr << "Failed: Model .obj not found.\n";
    exit(EXIT_FAILURE);
  }

  std::string line = "";
  while (!file_stream.eof()) {
    getline(file_stream, line);
    if (!line.compare(0, 2, "v ")) {
      std::stringstream ss(line.erase(0, 2));
      ss >> x;
      ss >> y;
      ss >> z;
      this->Indices.push_back(this->Vertices.size());
      this->Vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(x, y, z)));
    }
  }
}

Mesh::Mesh(const std::string &file_path) : file_path(file_path) {
  this->createVertices();
}