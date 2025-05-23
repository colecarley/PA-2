#pragma once

#include <vector>

#include "graphics_headers.h"

class Shader {
public:
  Shader();
  ~Shader();
  bool Initialize();
  void Enable();
  bool AddShader(GLenum ShaderType);
  bool AttachShader(std::string s, GLenum ShaderType);
  bool Finalize();
  GLint GetUniformLocation(const char *pUniformName);
  GLint GetAttribLocation(const char *pAttriName);

private:
  GLuint m_shaderProg;
  std::vector<GLuint> m_shaderObjList;
};
