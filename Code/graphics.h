#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "camera.h"
#include "cube.h"
#include "graphics_headers.h"
#include "object.h"
#include "pyramid.h"
#include "shader.h"
#include "sphere.h"
#include <vector>

class Graphics {
public:
  Graphics();
  ~Graphics();
  bool Initialize(int width, int height);
  void Update(double dt, glm::vec3 speed);
  void Render();

  Object *getInteractWith();

  Object *getSun() { return sun; }
  Object *getMoon() { return moon; }
  Object *getPlanet() { return planet; }
  void ComputeTransforms(double dt, std::vector<float> speed,
                         std::vector<float> dist, std::vector<float> rotSpeed,
                         glm::vec3 rotVector, std::vector<float> scale,
                         glm::mat4 &tmat, glm::mat4 &rmat, glm::mat4 &smat);

private:
  std::string ErrorString(GLenum error);

  Camera *m_camera;
  Shader *m_shader;

  GLint m_projectionMatrix;
  GLint m_viewMatrix;
  GLint m_modelMatrix;
  GLint m_vertPos;
  GLint m_vertCol;

  Object *sun;
  Object *planet;
  Object *planet2;
  Object *planet3;
  Object *planet4;
  Object *moon;
  Object *moon2;
};

#endif /* GRAPHICS_H */
