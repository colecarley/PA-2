#pragma once
#include "graphics_headers.h"
#include "shader.h"

struct ShaderVariableLocations {
  GLint m_projectionMatrix;
  GLint m_viewMatrix;
  GLint m_modelMatrix;
  GLint m_vertPos;
  GLint m_vertNorm;
  GLint m_vertText;
  GLint m_samplerLoc;
  GLint m_sampler2Loc;
  GLint is_emissive_loc;
  GLint light_pos_loc;
  GLint light_color_loc;
  GLint view_pos_loc;
  GLint has_normal_map_loc;
  GLint material_ambient_loc;
  GLint material_specular_loc;
  GLint material_diffuse_loc;
  GLint material_shininess_loc;
  GLint use_instancing_loc;
  ShaderVariableLocations() = default;
  void print() {
    std::cout << "projection matrix location: " << m_projectionMatrix
              << std::endl;
    std::cout << "view matrix location: " << m_viewMatrix << std::endl;
    std::cout << "model matrix location: " << m_modelMatrix << std::endl;
    std::cout << "vertex position location: " << m_vertPos << std::endl;
    std::cout << "vertex normal location: " << m_vertNorm << std::endl;
    std::cout << "vertex texture location: " << m_vertText << std::endl;
    std::cout << "sampler location: " << m_samplerLoc << std::endl;
    std::cout << "sampler2 location: " << m_sampler2Loc << std::endl;
    std::cout << "is_emissive location: " << is_emissive_loc << std::endl;
    std::cout << "light position location: " << light_pos_loc << std::endl;
    std::cout << "light color location: " << light_color_loc << std::endl;
    std::cout << "view position location: " << view_pos_loc << std::endl;
    std::cout << "has normal map location: " << has_normal_map_loc << std::endl;
    std::cout << "material ambient location: " << material_ambient_loc
              << std::endl;
    std::cout << "material specular location: " << material_specular_loc
              << std::endl;
    std::cout << "material diffuse locatioin: " << material_diffuse_loc
              << std::endl;
    std::cout << "material shininess location:" << material_shininess_loc
              << std::endl;
    std::cout << "use instancing location:" << use_instancing_loc << std::endl;
  }
};
