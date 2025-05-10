#include "skybox.h"
#include "/usr/local/include/soil2/SOIL2.h"
#include "graphics_headers.h"
#include <cstdlib>

Skybox::Skybox(const std::array<std::string, 6> &cubemap_path) {
  this->create_vertices();
  this->initialize();
  this->initialize_shaders();
  this->add_textures(cubemap_path);
};

void Skybox::initialize() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &VB);
  glGenBuffers(1, &IB);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
               &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               &indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::create_vertices() {
  vertices = {glm::vec3{1.0f, -1.0f, -1.0f}, glm::vec3{1.0f, -1.0f, 1.0f},
              glm::vec3{-1.0f, -1.0f, 1.0f}, glm::vec3{-1.0f, -1.0f, -1.0f},
              glm::vec3{1.0f, 1.0f, -1.0f},  glm::vec3{1.0f, 1.0f, 1.0f},
              glm::vec3{-1.0f, 1.0f, 1.0f},  glm::vec3{-1.0f, 1.0f, -1.0f}};

  indices = {1, 2, 3, 7, 6, 5, 0, 4, 5, 1, 5, 6, 6, 7, 3, 0, 3, 7,
             0, 1, 3, 4, 7, 5, 1, 0, 5, 2, 1, 6, 2, 6, 3, 4, 0, 7};
}

void Skybox::add_textures(const std::array<std::string, 6> &cubemap_paths) {
  this->texture_id = SOIL_load_OGL_cubemap(
      cubemap_paths.at(0).c_str(), cubemap_paths.at(1).c_str(),
      cubemap_paths.at(2).c_str(), cubemap_paths.at(3).c_str(),
      cubemap_paths.at(4).c_str(), cubemap_paths.at(5).c_str(), SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

  if (texture_id == 0) {
    std::cout << "SOIL error: " << SOIL_last_result() << std::endl;
    std::cerr << "Failed to initialize cubemap" << std::endl;
    auto error = glGetError();
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Skybox::initialize_shaders() {
  this->shader = std::make_unique<Shader>();
  if (!this->shader->Initialize()) {
    std::cerr << "skybox shader failed to initialize" << std::endl;
    return;
  }

  std::string v = "#version 410\n \
															 layout (location = 0) in vec3 aPos;\
															 uniform mat4 projection;\
															 uniform mat4 view;\
															 out vec3 tex_coord;\
															 void main() {\
																 tex_coord = aPos;\
																 gl_Position = projection * view * vec4(aPos, 1.0);\
															 }\
                              ";
  if (!this->shader->AttachShader(v, GL_VERTEX_SHADER)) {
    std::cerr << "Skybox vertex shader failed to initialize" << std::endl;
    return;
  }

  std::string f = "#version 410\n \
																 out vec4 frag_color;\
																 in vec3 tex_coord;\
																 uniform samplerCube skybox;\
																 void main() {\
																	 frag_color = texture(skybox, tex_coord);\
																 }\
																 ";
  this->shader->AttachShader(f, GL_FRAGMENT_SHADER);

  if (!this->shader->Finalize()) {
    std::cerr << "skybox shader failed to finalize" << std::endl;
    return;
  }
}

void Skybox::render(glm::mat4 projection, glm::mat4 view) {
  // bind VAO
  glBindVertexArray(vao);

  // bind VBO(s)
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glEnableVertexAttribArray(0);

  shader->Enable();
  glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
                     glm::value_ptr(projection));
  glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
                     glm::value_ptr(glm::mat4(glm::mat3(view))));
  glUniform1i(shader->GetUniformLocation("skybox"), 0);

  glDepthMask(GL_FALSE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  // draw call to OpenGL
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);

  // unbind VBO(s) and ElementBuffer(s)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDepthMask(GL_TRUE);
}

std::unique_ptr<Shader> &Skybox::get_shader() { return this->shader; }
