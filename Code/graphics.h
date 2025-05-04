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
  void Update(double dt, glm::vec3 speed);
  void Render();

  std::unique_ptr<Object> &getInteractWith();
  std::unique_ptr<Object> &getSun() { return sun; }
  std::unique_ptr<Object> &getMoon() { return moon; }
  std::unique_ptr<Object> &getPlanet() { return planet; }
  std::unique_ptr<Camera> &getCamera() { return m_camera; }
  void ComputeTransforms(double dt, std::vector<float> speed,
                         std::vector<float> dist, std::vector<float> rotSpeed,
                         glm::vec3 rotVector, std::vector<float> scale,
                         glm::mat4 &tmat, glm::mat4 &rmat, glm::mat4 &smat);

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

  std::unique_ptr<Object> sun;
  std::unique_ptr<Object> planet;
  std::unique_ptr<Object> moon;
  std::unique_ptr<Object> ship;
};
