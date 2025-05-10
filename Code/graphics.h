#pragma once

#include "camera.h"
#include "graphics_headers.h"
#include "mode.h"
#include "object.h"
#include "shader.h"
#include "skybox.h"
#include <memory>
#include <vector>

class Graphics {
public:
  Graphics();
  ~Graphics();
  bool Initialize(int width, int height);
  void Update(double dt, Mode mode);
  void Render(Mode mode);

  std::unique_ptr<Camera> &getCamera();

private:
  std::string ErrorString(GLenum error);

  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<Shader> m_shader;

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

  std::unique_ptr<Object> sun;
  std::unique_ptr<Object> planet;
  std::unique_ptr<Object> moon;
  std::unique_ptr<Object> ship;

  std::unique_ptr<Shader> skybox_shader;
  std::unique_ptr<Skybox> skybox;
};
