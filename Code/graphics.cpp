#include "graphics.h"
#include "graphics_headers.h"
#include "mesh.h"
#include "mode.h"
#include "sphere.h"
#include "ring.h"
#include <memory>
#include <stack>

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
  m_projectionMatrix = m_shader->GetUniformLocation("projection");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("view");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("model");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_samplerLoc = m_shader->GetUniformLocation("samp");
  if (m_samplerLoc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "m_samplerLoc not found\n";
    return false;
  }

  m_sampler2Loc = m_shader->GetUniformLocation("samp2");
  if (m_sampler2Loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "m_samplerLoc not found\n";
    return false;
  }

  is_emissive_loc = m_shader->GetUniformLocation("is_emissive");
  if (is_emissive_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "is_emissive_loc not found\n";
    return false;
  }

  light_pos_loc = m_shader->GetUniformLocation("light_pos");
  if (light_pos_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "light_pos_loc not found\n";
    return false;
  }

  light_color_loc = m_shader->GetUniformLocation("light_color");
  if (light_color_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "light_color_loc not found\n";
    return false;
  }

  view_pos_loc = m_shader->GetUniformLocation("view_pos");
  if (view_pos_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "view_pos_loc not found\n";
    return false;
  }

  has_normal_map_loc = m_shader->GetUniformLocation("has_normal_map");
  if (has_normal_map_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "has_normal_map_loc not found\n";
    return false;
  }

  material_ambient_loc = m_shader->GetUniformLocation("material.ambient");
  if (material_ambient_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  material_diffuse_loc = m_shader->GetUniformLocation("material.diffuse");
  if (material_diffuse_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  material_specular_loc = m_shader->GetUniformLocation("material.specular");
  if (material_specular_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  material_shininess_loc = m_shader->GetUniformLocation("material.shininess");
  if (material_shininess_loc == INVALID_UNIFORM_LOCATION) {
    std::cerr << "material_loc not found\n";
    return false;
  }

  use_instancing_loc = m_shader->GetUniformLocation("use_instancing");
  if (use_instancing_loc == INVALID_UNIFORM_LOCATION) {
      std::cerr << "use_instancing not found\n";
      return false;
  }


  // Find where vertex attributes are in shader
  m_vertPos = m_shader->GetAttribLocation("v_position");
  m_vertNorm = m_shader->GetAttribLocation("v_normal");
  m_vertText = m_shader->GetAttribLocation("v_texture");

  // Create objects

  asteroid = std::make_shared<Mesh>("../assets/Asteroid.obj", "../assets/2k_sun.jpg");
  asteroid->Initialize(m_vertPos, m_vertNorm, m_vertText);

  asteroid_belt = std::make_unique<AsteroidBelt>(std::dynamic_pointer_cast<Mesh>(asteroid));
  asteroid_belt->Initialize();

  ship = std::make_unique<Mesh>("../assets/SpaceShip-1.obj",
      "../assets/SpaceShip-1.png");
  ship->Initialize(m_vertPos, m_vertNorm, m_vertText);

  sun = std::make_unique<Sphere>("../assets/planetary_textures/2k_sun.jpg");
  sun->Initialize(m_vertPos, m_vertNorm, m_vertText);

  mercury = std::make_unique<Sphere>(
      "../assets/planetary_textures/Mercury.jpg",
      "../assets/planetary_textures/Mercury-n.jpg");
  mercury->Initialize(m_vertPos, m_vertNorm, m_vertText);

  venus = std::make_unique<Sphere>(
      "../assets/planetary_textures/Venus.jpg",
      "../assets/planetary_textures/Venus-n.jpg");
  venus->Initialize(m_vertPos, m_vertNorm, m_vertText);

  earth = std::make_unique<Sphere>(
      "../assets/planetary_textures/2k_earth_daymap.jpg",
      "../assets/planetary_textures/2k_earth_daymap-n.jpg");
  earth->Initialize(m_vertPos, m_vertNorm, m_vertText);

  moon = std::make_unique<Sphere>("../assets/planetary_textures/2k_moon.jpg",
                                  "../assets/planetary_textures/2k_moon-n.jpg");
  moon->Initialize(m_vertPos, m_vertNorm, m_vertText);

  mars = std::make_unique<Sphere>("../assets/planetary_textures/Mars.jpg",
      "../assets/planetary_textures/Mars-n.jpg");
  mars->Initialize(m_vertPos, m_vertNorm, m_vertText);

  phobos = std::make_unique<Sphere>("../assets/planetary_textures/Phobos.jpg",
          "../assets/planetary_textures/Phobos-n.jpg");
  phobos->Initialize(m_vertPos, m_vertNorm, m_vertText);

  jupiter = std::make_unique<Sphere>("../assets/planetary_textures/Jupiter.jpg",
      "../assets/planetary_textures/Jupiter-n.jpg");
  jupiter->Initialize(m_vertPos, m_vertNorm, m_vertText);

  io = std::make_unique<Sphere>("../assets/planetary_textures/Io.jpg",
      "../assets/planetary_textures/Io-n.jpg");
  io->Initialize(m_vertPos, m_vertNorm, m_vertText);

  saturn = std::make_unique<Sphere>("../assets/planetary_textures/Saturn.jpg",
      "../assets/planetary_textures/Saturn-n.jpg");
  saturn->Initialize(m_vertPos, m_vertNorm, m_vertText);
  
  saturn_ring = std::make_unique<Ring>("../assets/planetary_textures/Saturn_ring.jpg","../assets/planetary_textures/Saturn_ring-n.jpg");
  saturn_ring->Initialize(m_vertPos, m_vertNorm, m_vertText);

  titan = std::make_unique<Sphere>("../assets/planetary_textures/Titan.jpg",
          "../assets/planetary_textures/Titan-n.jpg");
  titan->Initialize(m_vertPos, m_vertNorm, m_vertText);

  uranus = std::make_unique<Sphere>("../assets/planetary_textures/Uranus.jpg",
      "../assets/planetary_textures/Uranus-n.jpg");
  uranus->Initialize(m_vertPos, m_vertNorm, m_vertText);

  neptune = std::make_unique<Sphere>("../assets/planetary_textures/Neptune.jpg",
      "../assets/planetary_textures/Neptune-n.jpg");
  neptune->Initialize(m_vertPos, m_vertNorm, m_vertText);

  // enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::updateOrbitalBody(Object* obj, double dt, const OrbitalBody& body) {
    float orbit_angle = static_cast<float>(dt) * body.orbit_speed;
    float spin_angle = static_cast<float>(dt) * body.spin_speed;

    // Handle orbit direction (e.g., Moon uses negative cosine/sine)
    float x = body.use_negative_orbit ? -cos(orbit_angle) : cos(orbit_angle);
    float z = body.use_negative_orbit ? -sin(orbit_angle) : sin(orbit_angle);
    float y = sin(static_cast<float>(dt)) * body.vertical_oscillation_amplitude;

    glm::mat4 tmat = glm::translate(glm::mat4(1), glm::vec3(x * body.orbit_radius, y, z * body.orbit_radius));
    glm::mat4 rmat = body.skip_spin ? glm::mat4(1) : glm::rotate(glm::mat4(1), spin_angle, glm::vec3(0, 1, 0));
    glm::mat4 smat = glm::scale(glm::vec3(body.scale));

    obj->Update(tmat * rmat * smat);
}

void Graphics::Update(double dt, Mode mode) {
    glm::mat4 tmat, rmat, smat;

    if (mode == PLANETARY_OBSERVATION) {
        float cam_orbit_radius = 1.0f;
        float cam_orbit_speed = 0.2f;
        glm::vec3 earth_pos = glm::vec3((sun->GetModel() * earth->GetModel())[3]);
        glm::vec3 camera_offset =
            glm::vec3{ cos(dt * cam_orbit_speed) * cam_orbit_radius,
                      1.0f, // slightly above
                      sin(dt * cam_orbit_speed) * cam_orbit_radius };
        glm::vec3 camera_pos = earth_pos + camera_offset;

        this->m_camera->set_position(camera_pos);
        this->m_camera->set_front(earth_pos);
        this->m_camera->set_view(glm::lookAt(this->m_camera->get_position(),
            this->m_camera->get_front(),
            this->m_camera->get_up()));
    }
    else {
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
  
    updateOrbitalBody(sun.get(), dt, SolarSystem::Sun);
    updateOrbitalBody(mercury.get(), dt, SolarSystem::Mercury);
    updateOrbitalBody(venus.get(), dt, SolarSystem::Venus);
    updateOrbitalBody(earth.get(), dt, SolarSystem::Earth);
    updateOrbitalBody(moon.get(), dt, SolarSystem::Moon);
    updateOrbitalBody(mars.get(), dt, SolarSystem::Mars);
    updateOrbitalBody(phobos.get(), dt, SolarSystem::Phobos);
    updateOrbitalBody(jupiter.get(), dt, SolarSystem::Jupiter);
    updateOrbitalBody(io.get(), dt, SolarSystem::Io);
    updateOrbitalBody(saturn.get(), dt, SolarSystem::Saturn);
    updateOrbitalBody(saturn_ring.get(), dt, SolarSystem::Saturn); // same transform as saturn
    updateOrbitalBody(titan.get(), dt, SolarSystem::Titan);
    updateOrbitalBody(uranus.get(), dt, SolarSystem::Uranus);
    updateOrbitalBody(neptune.get(), dt, SolarSystem::Neptune);

}

void Graphics::Render(Mode mode) {
  // clear the screen
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // enable the skybox shader
  skybox->render(m_camera->GetProjection(), m_camera->GetView());

  // Start the correct program
  m_shader->Enable();
  glUniform1i(use_instancing_loc, 0); // no instancing default

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetView()));
  glUniform3fv(view_pos_loc, 1, glm::value_ptr(m_camera->get_position()));


  // Ship
  if (mode == EXPLORATION) {
      glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
          glm::value_ptr(ship->GetModel()));
      ship->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
          light_pos_loc, light_color_loc, has_normal_map_loc,
          material_ambient_loc, material_specular_loc,
          material_diffuse_loc, material_shininess_loc, is_emissive_loc,
          false);
  }

  // Asteroid Belt (NOT WORKING??)

  asteroid_belt->Render(m_projectionMatrix, m_viewMatrix, use_instancing_loc,
      m_camera->GetProjection(), m_camera->GetView());
  
  // Hierarchical Updates

  std::stack<glm::mat4> stack;
  stack.push(glm::mat4(1.0f)); // IMat

  // Sun
  stack.push(stack.top() * sun->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  sun->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
              light_pos_loc, light_color_loc, has_normal_map_loc,
              material_ambient_loc, material_specular_loc, material_diffuse_loc,
              material_shininess_loc, is_emissive_loc, true);

  // Mercury 
  stack.push(stack.top() * mercury->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  mercury->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();
  
  // Venus
  stack.push(stack.top() * venus->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  venus->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();

  // Earth
  stack.push(stack.top() * earth->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  earth->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
                 light_pos_loc, light_color_loc, has_normal_map_loc,
                 material_ambient_loc, material_specular_loc,
                 material_diffuse_loc, material_shininess_loc, is_emissive_loc,
                 false);

  // Moon
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * moon->GetModel()));
  moon->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
               light_pos_loc, light_color_loc, has_normal_map_loc,
               material_ambient_loc, material_specular_loc,
               material_diffuse_loc, material_shininess_loc, is_emissive_loc,
               false);
  stack.pop();

  // Mars
  stack.push(stack.top() * mars->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  mars->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);

  // Phobos
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
      glm::value_ptr(stack.top() * phobos->GetModel()));
  phobos->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();

  // Jupiter
  stack.push(stack.top() * jupiter->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  jupiter->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);

  // Io
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
      glm::value_ptr(stack.top() * io->GetModel()));
  io->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();

  // Saturn
  stack.push(stack.top() * saturn->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  saturn->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);

  // Ring
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top())); 
  saturn_ring->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);

  // Titan
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
      glm::value_ptr(stack.top()* titan->GetModel()));
  titan->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();

  // Uranus
  stack.push(stack.top() * uranus->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  uranus->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();

  // Neptune
  stack.push(stack.top()* neptune->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  neptune->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
      light_pos_loc, light_color_loc, has_normal_map_loc,
      material_ambient_loc, material_specular_loc,
      material_diffuse_loc, material_shininess_loc, is_emissive_loc,
      false);
  stack.pop();


  stack.pop(); // Sun
  stack.pop(); // IMat




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
