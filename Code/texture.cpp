#include "texture.h"
#include "SOIL2/SOIL2.h"

Texture::Texture() {}

Texture::Texture(const std::string filename) { this->load_texture(filename); }

bool Texture::load_texture(const std::string filename) {
  this->m_texture_id = SOIL_load_OGL_texture(
      filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

  if (!this->m_texture_id) {
    std::cerr << "Failed: could not open texture file: " << filename << "!"
              << std::endl;
    return false;
  }
  return true;
}

GLint Texture::get_texture_id() { return this->m_texture_id; }
