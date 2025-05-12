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
  GLint instance_mat_0;
  GLint instance_mat_1;
  GLint instance_mat_2;
  GLint instance_mat_3;
  ShaderVariableLocations() = default;
};
