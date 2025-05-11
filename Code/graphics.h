#pragma once

#include "asteroid_belt.h"
#include "camera.h"
#include "graphics_headers.h"
#include "mode.h"
#include "object.h"
#include "shader.h"
#include "shader_variable_locations.h"
#include "skybox.h"
#include "solar_constants.h"
#include <memory>

class Graphics {
public:
  Graphics();
  ~Graphics();
  bool Initialize(int width, int height);
  void Update(double dt, Mode mode, Planet focused_planet);
  void Render(Mode mode);
  std::unique_ptr<Camera> &getCamera();

private:
  std::unique_ptr<Object> &get_planet_model(Planet focused_planet);
  void updateOrbitalBody(std::unique_ptr<Object> &obj, double dt,
                         const OrbitalBody &body);
  std::string ErrorString(GLenum error);

  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<Shader> m_shader;
  ShaderVariableLocations shader_var_locs;

  std::unique_ptr<Object> ship;
  std::shared_ptr<Object> asteroid;
  std::unique_ptr<AsteroidBelt> asteroid_belt;
  std::unique_ptr<Object> sun;
  std::unique_ptr<Object> mercury;
  std::unique_ptr<Object> venus;
  std::unique_ptr<Object> earth;
  std::unique_ptr<Object> moon;
  std::unique_ptr<Object> mars;
  std::unique_ptr<Object> io; // mars moon
  std::unique_ptr<Object> jupiter;
  std::unique_ptr<Object> phobos; // jupiter moon
  std::unique_ptr<Object> saturn;
  std::unique_ptr<Object> saturn_ring;
  std::unique_ptr<Object> titan; // asturn moon
  std::unique_ptr<Object> uranus;
  std::unique_ptr<Object> neptune;

  std::unique_ptr<Shader> skybox_shader;
  std::unique_ptr<Skybox> skybox;
};
