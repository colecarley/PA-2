#pragma once

#include "camera.h"
#include "graphics_headers.h"
#include "object.h"
#include "shader.h"
#include <memory>
#include <vector>

class Graphics {
public:
  Graphics();
  ~Graphics();
  bool Initialize(int width, int height);
  void Update(double dt);
  void Render();

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

  std::unique_ptr<Object> sun;
  std::unique_ptr<Object> planet;
  std::unique_ptr<Object> moon;
  std::unique_ptr<Object> ship;
};
