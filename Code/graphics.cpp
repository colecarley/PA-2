#include "graphics.h"
#include "graphics_headers.h"
#include "mesh.h"
#include "mode.h"
#include "ring.h"
#include "sphere.h"
#include <exception>
#include <memory>
#include <stack>
#include <stdexcept>

Graphics::Graphics() {}
Graphics::~Graphics() {}

bool Graphics::Initialize(int width, int height) {
  // Init Camera
  m_camera = std::make_unique<Camera>();
  if (!m_camera->Initialize(width, height)) {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Init skybox
  skybox = std::make_unique<Skybox>(std::array<std::string, 6>{
      "../assets/skybox/right.png",
      "../assets/skybox/left.png",
      "../assets/skybox/top.png",
      "../assets/skybox/bottom.png",
      "../assets/skybox/front.png",
      "../assets/skybox/back.png",
  });

  // Set up the shaders
  m_shader = std::make_unique<Shader>();
  if (!m_shader->Initialize()) {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if (!m_shader->AddShader(GL_VERTEX_SHADER)) {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if (!m_shader->AddShader(GL_FRAGMENT_SHADER)) {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if (!m_shader->Finalize()) {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  shader_var_locs.m_projectionMatrix =
      m_shader->GetUniformLocation("projection");
  if (shader_var_locs.m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  shader_var_locs.m_viewMatrix = m_shader->GetUniformLocation("view");
  if (shader_var_locs.m_viewMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  shader_var_locs.m_modelMatrix = m_shader->GetUniformLocation("model");
  if (shader_var_locs.m_modelMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_modelMatrix not found\n");
    return false;
  }

  shader_var_locs.m_samplerLoc = m_shader->GetUniformLocation("samp");
  if (shader_var_locs.m_samplerLoc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "m_samplerLoc not found\n";
    return false;
  }

  shader_var_locs.m_sampler2Loc = m_shader->GetUniformLocation("samp2");
  if (shader_var_locs.m_sampler2Loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "m_samplerLoc not found\n";
    return false;
  }

  shader_var_locs.is_emissive_loc = m_shader->GetUniformLocation("is_emissive");
  if (shader_var_locs.is_emissive_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "is_emissive_loc not found\n";
    return false;
  }

  shader_var_locs.light_pos_loc = m_shader->GetUniformLocation("light_pos");
  if (shader_var_locs.light_pos_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "light_pos_loc not found\n";
    return false;
  }

  shader_var_locs.light_color_loc = m_shader->GetUniformLocation("light_color");
  if (shader_var_locs.light_color_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "light_color_loc not found\n";
    return false;
  }

  shader_var_locs.view_pos_loc = m_shader->GetUniformLocation("view_pos");
  if (shader_var_locs.view_pos_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "view_pos_loc not found\n";
    return false;
  }

  shader_var_locs.has_normal_map_loc =
      m_shader->GetUniformLocation("has_normal_map");
  if (shader_var_locs.has_normal_map_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "has_normal_map_loc not found\n";
    return false;
  }

  shader_var_locs.material_ambient_loc =
      m_shader->GetUniformLocation("material.ambient");
  if (shader_var_locs.material_ambient_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  shader_var_locs.material_diffuse_loc =
      m_shader->GetUniformLocation("material.diffuse");
  if (shader_var_locs.material_diffuse_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  shader_var_locs.material_specular_loc =
      m_shader->GetUniformLocation("material.specular");
  if (shader_var_locs.material_specular_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  shader_var_locs.material_shininess_loc =
      m_shader->GetUniformLocation("material.shininess");
  if (shader_var_locs.material_shininess_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  shader_var_locs.use_instancing_loc =
      m_shader->GetUniformLocation("use_instancing");
  if (shader_var_locs.use_instancing_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "use_instancing not found\n";
    return false;
  }

  // Find where vertex attributes are in shader
  shader_var_locs.m_vertPos = m_shader->GetAttribLocation("v_position");
  shader_var_locs.m_vertNorm = m_shader->GetAttribLocation("v_normal");
  shader_var_locs.m_vertText = m_shader->GetAttribLocation("v_texture");
  shader_var_locs.instance_mat_0 =
      m_shader->GetAttribLocation("instance_mat_0");
  shader_var_locs.instance_mat_1 =
      m_shader->GetAttribLocation("instance_mat_1");
  shader_var_locs.instance_mat_2 =
      m_shader->GetAttribLocation("instance_mat_2");
  shader_var_locs.instance_mat_3 =
      m_shader->GetAttribLocation("instance_mat_3");

  // Create objects

  asteroid = std::make_shared<Mesh>("../assets/Asteroid2.obj",
      "../assets/Asteroid.jpg");
  asteroid->Initialize(shader_var_locs);

  asteroid_belt = std::make_unique<AsteroidBelt>(
      asteroid, SolarSystem::MainAsteroidBelt);
  asteroid_belt->Initialize();

  ship = std::make_unique<Mesh>("../assets/SpaceShip-1.obj",
                                "../assets/SpaceShip-1.png");
  ship->Initialize(shader_var_locs);

  sun = std::make_unique<Sphere>("../assets/planetary_textures/2k_sun.jpg");
  sun->Initialize(shader_var_locs);

  mercury =
      std::make_unique<Sphere>("../assets/planetary_textures/Mercury.jpg",
                               "../assets/planetary_textures/Mercury-n.jpg");
  mercury->Initialize(shader_var_locs);

  venus = std::make_unique<Sphere>("../assets/planetary_textures/Venus.jpg",
                                   "../assets/planetary_textures/Venus-n.jpg");
  venus->Initialize(shader_var_locs);

  earth = std::make_unique<Sphere>(
      "../assets/planetary_textures/2k_earth_daymap.jpg",
      "../assets/planetary_textures/2k_earth_daymap-n.jpg");
  earth->Initialize(shader_var_locs);

  moon = std::make_unique<Sphere>("../assets/planetary_textures/2k_moon.jpg",
                                  "../assets/planetary_textures/2k_moon-n.jpg");
  moon->Initialize(shader_var_locs);

  mars = std::make_unique<Sphere>("../assets/planetary_textures/Mars.jpg",
                                  "../assets/planetary_textures/Mars-n.jpg");
  mars->Initialize(shader_var_locs);

  phobos =
      std::make_unique<Sphere>("../assets/planetary_textures/Phobos.jpg",
                               "../assets/planetary_textures/Phobos-n.jpg");
  phobos->Initialize(shader_var_locs);

  jupiter =
      std::make_unique<Sphere>("../assets/planetary_textures/Jupiter.jpg",
                               "../assets/planetary_textures/Jupiter-n.jpg");
  jupiter->Initialize(shader_var_locs);

  io = std::make_unique<Sphere>("../assets/planetary_textures/Io.jpg",
                                "../assets/planetary_textures/Io-n.jpg");
  io->Initialize(shader_var_locs);

  saturn =
      std::make_unique<Sphere>("../assets/planetary_textures/Saturn.jpg",
                               "../assets/planetary_textures/Saturn-n.jpg");
  saturn->Initialize(shader_var_locs);

  saturn_ring =
      std::make_unique<Ring>("../assets/planetary_textures/Saturn_ring.jpg",
                             "../assets/planetary_textures/Saturn_ring-n.jpg");
  saturn_ring->Initialize(shader_var_locs);

  titan = std::make_unique<Sphere>("../assets/planetary_textures/Titan.jpg",
                                   "../assets/planetary_textures/Titan-n.jpg");
  titan->Initialize(shader_var_locs);

  uranus =
      std::make_unique<Sphere>("../assets/planetary_textures/Uranus.jpg",
                               "../assets/planetary_textures/Uranus-n.jpg");
  uranus->Initialize(shader_var_locs);

  neptune =
      std::make_unique<Sphere>("../assets/planetary_textures/Neptune.jpg",
                               "../assets/planetary_textures/Neptune-n.jpg");
  neptune->Initialize(shader_var_locs);

  // enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::updateOrbitalBody(std::unique_ptr<Object> &obj, double dt,
                                 const OrbitalBody &body) {
  float orbit_angle = (float)dt * body.orbit_speed;
  float spin_angle = (float)dt * body.spin_speed;

  // Handle orbit direction (e.g., Moon uses negative cosine/sine)
  float x = body.use_negative_orbit ? -cos(orbit_angle) : cos(orbit_angle);
  float z = body.use_negative_orbit ? -sin(orbit_angle) : sin(orbit_angle);
  float y = sin((float)dt) * body.vertical_oscillation_amplitude;

  glm::mat4 tmat = glm::translate(
      glm::mat4(1), glm::vec3(x * body.orbit_radius, y, z * body.orbit_radius));
  glm::mat4 rmat = body.skip_spin ? glm::mat4(1)
                                  : glm::rotate(glm::mat4(1), spin_angle,
                                                glm::vec3(0, 1, 0));
  glm::mat4 smat = glm::scale(glm::vec3(body.scale));

  obj->Update(tmat * rmat * smat);
}

std::unique_ptr<Object> &Graphics::get_planet_model(Planet focused_planet) {
  switch (focused_planet) {
  case MERCURY:
    return this->mercury;
  case VENUS:
    return this->venus;
  case EARTH:
    return this->earth;
  case MARS:
    return this->mars;
  case JUPITER:
    return this->jupiter;
  case SATURN:
    return this->saturn;
  case URANUS:
    return this->uranus;
  case NEPTUNE:
    return this->neptune;
  default:
    throw std::invalid_argument("Invalid planet");
  }
}

void Graphics::Update(double dt, Mode mode, Planet focused_planet) {
  glm::mat4 tmat, rmat, smat;

  if (mode == PLANETARY_OBSERVATION) {
    float cam_orbit_radius = 1.5f;
    glm::vec3 earth_pos = glm::vec3(
        (sun->GetModel() * get_planet_model(focused_planet)->GetModel())[3]);
    glm::vec3 camera_offset = glm::vec3{cam_orbit_radius, 0, 0};
    glm::vec3 camera_pos = earth_pos + camera_offset;

    this->m_camera->set_position(camera_pos);
    this->m_camera->set_front(earth_pos);
    this->m_camera->set_view(glm::lookAt(this->m_camera->get_position(),
                                         this->m_camera->get_front(),
                                         this->m_camera->get_up()));
  } else {
    tmat = glm::translate(glm::mat4(1),
                          this->m_camera->get_position() +
                              glm::normalize(this->m_camera->get_front()));
    tmat = glm::translate(tmat, glm::vec3(0, -0.3, 0));
    smat = glm::scale(glm::vec3(0.02f));
    rmat = glm::rotate(
        glm::mat4(glm::mat3(glm::transpose(this->m_camera->GetView()))),
        glm::radians(20.0f), glm::vec3(1, 0, 0));
    rmat = glm::rotate(rmat, glm::radians(180.0f), glm::vec3(0, 1, 0));

    ship->Update(tmat * rmat * smat);
  }

  // Celestial Bodies
  updateOrbitalBody(sun, dt, SolarSystem::Sun);
  updateOrbitalBody(mercury, dt, SolarSystem::Mercury);
  updateOrbitalBody(venus, dt, SolarSystem::Venus);
  updateOrbitalBody(earth, dt, SolarSystem::Earth);
  updateOrbitalBody(moon, dt, SolarSystem::Moon);
  updateOrbitalBody(mars, dt, SolarSystem::Mars);
  updateOrbitalBody(phobos, dt, SolarSystem::Phobos);
  updateOrbitalBody(jupiter, dt, SolarSystem::Jupiter);
  updateOrbitalBody(io, dt, SolarSystem::Io);
  updateOrbitalBody(saturn, dt, SolarSystem::Saturn);
  // same transform as saturn
  updateOrbitalBody(saturn_ring, dt, SolarSystem::Saturn);
  updateOrbitalBody(titan, dt, SolarSystem::Titan);
  updateOrbitalBody(uranus, dt, SolarSystem::Uranus);
  updateOrbitalBody(neptune, dt, SolarSystem::Neptune);
}

void Graphics::Render(Mode mode) {
  // clear the screen
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // enable the skybox shader
  skybox->render(m_camera->GetProjection(), m_camera->GetView());

  // Start the correct program
  m_shader->Enable();
  glUniform1i(shader_var_locs.use_instancing_loc, 0); // no instancing default

  // Send in the projection and view to the shader
  glUniformMatrix4fv(shader_var_locs.m_projectionMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(shader_var_locs.m_viewMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetView()));
  glUniform3fv(shader_var_locs.view_pos_loc, 1,
               glm::value_ptr(m_camera->get_position()));

  // Ship
  if (mode == EXPLORATION) {
    glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                       glm::value_ptr(ship->GetModel()));
    ship->Render(shader_var_locs);
  }


  // Hierarchical Updates

  std::stack<glm::mat4> stack;
  stack.push(glm::mat4(1.0f)); // IMat

  // Sun
  stack.push(stack.top() * sun->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  sun->Render(shader_var_locs, true);

  // Mercury
  stack.push(stack.top() * mercury->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  mercury->Render(shader_var_locs);
  stack.pop();

  // Venus
  stack.push(stack.top() * venus->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  venus->Render(shader_var_locs);
  stack.pop();

  // Earth
  stack.push(stack.top() * earth->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  earth->Render(shader_var_locs);

  // Moon
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * moon->GetModel()));
  moon->Render(shader_var_locs);
  stack.pop();

  // Mars
  stack.push(stack.top() * mars->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  mars->Render(shader_var_locs);

  // Phobos
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * phobos->GetModel()));
  phobos->Render(shader_var_locs);
  stack.pop();

  // Jupiter
  stack.push(stack.top() * jupiter->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  jupiter->Render(shader_var_locs);

  // Io
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * io->GetModel()));
  io->Render(shader_var_locs);
  stack.pop();

  // Saturn
  stack.push(stack.top() * saturn->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  saturn->Render(shader_var_locs);

  // Ring
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  saturn_ring->Render(shader_var_locs);

  // Titan
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * titan->GetModel()));
  titan->Render(shader_var_locs);
  stack.pop();

  // Uranus
  stack.push(stack.top() * uranus->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  uranus->Render(shader_var_locs);
  stack.pop();

  // Neptune
  stack.push(stack.top() * neptune->GetModel());
  glUniformMatrix4fv(shader_var_locs.m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top()));
  neptune->Render(shader_var_locs);
  stack.pop();

  stack.pop(); // Sun
  stack.pop(); // IMat

  // Asteroid Belt

  glUniform1i(shader_var_locs.use_instancing_loc, 1); // enable use_instancing

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, asteroid->getTextureID());
  glUniform1i(shader_var_locs.m_samplerLoc, 0);

  asteroid_belt->Render(shader_var_locs.m_projectionMatrix,
      shader_var_locs.m_viewMatrix,
      shader_var_locs.use_instancing_loc,
      m_camera->GetProjection(),
      m_camera->GetView());

  glUniform1i(shader_var_locs.use_instancing_loc, 0); // disable use_instancing

  // Get any errors from OpenGL
  auto error = glGetError();
  if (error != GL_NO_ERROR) {
    std::string val = ErrorString(error);
    std::cout << "Error initializing OpenGL! " << error << ", " << val
              << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error) {
  if (error == GL_INVALID_ENUM) {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an "
           "enumerated argument.";
  }

  else if (error == GL_INVALID_VALUE) {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if (error == GL_INVALID_OPERATION) {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in "
           "the current state.";
  }

  else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not "
           "complete.";
  }

  else if (error == GL_OUT_OF_MEMORY) {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the "
           "command.";
  } else {
    return "None";
  }
}

std::unique_ptr<Camera> &Graphics::getCamera() { return this->m_camera; }
