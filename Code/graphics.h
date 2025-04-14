#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "camera.h"
#include "cube.h"
#include "graphics_headers.h"
#include "object.h"
#include "shader.h"

class Graphics {
public:
  Graphics();
  ~Graphics();
  bool Initialize(int width, int height);
  void Update(unsigned int dt, glm::vec3 speed);
  void Render();

  Object *getInteractWith();

  Object *getObj() { return m_obj; }

private:
  std::string ErrorString(GLenum error);

  Camera *m_camera;
  Shader *m_shader;

  GLint m_projectionMatrix;
  GLint m_viewMatrix;
  GLint m_modelMatrix;
  GLint m_vertPos;
  GLint m_vertCol;

  Object *m_obj;
};

#endif /* GRAPHICS_H */
