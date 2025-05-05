#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void Mesh::createVertices() {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(mesh_file_path, aiProcess_Triangulate);

  aiMesh *mesh = scene->mMeshes[0];

  // Process vertices
  for (int i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D pos = mesh->mVertices[i];
    aiVector3D norm = mesh->mNormals[i];
    aiVector3D texCoord = mesh->mTextureCoords[0][i];

    Vertices.push_back(Vertex(glm::vec3(pos.x, pos.y, pos.z),
                              glm::vec3(norm.x, norm.y, norm.z),
                              glm::vec2(texCoord.x, texCoord.y)));
  }

  // Process indices
  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++) {
      Indices.push_back(face.mIndices[j]);
    }
  }
}

Mesh::Mesh(const std::string &mesh_file_path,
           const std::string &texture_file_path)
    : mesh_file_path(mesh_file_path) {
  this->texture = std::make_unique<Texture>(texture_file_path);
  this->createVertices();
}
