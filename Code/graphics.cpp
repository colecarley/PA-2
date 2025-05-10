#include "graphics.h"
#include "graphics_headers.h"
#include "mesh.h"
#include "sphere.h"
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

  // Find where vertex attributes are in shader
  m_vertPos = m_shader->GetAttribLocation("v_position");
  m_vertNorm = m_shader->GetAttribLocation("v_normal");
  m_vertText = m_shader->GetAttribLocation("v_texture");

  // Create the object
  sun = std::make_unique<Sphere>("../assets/planetary_textures/2k_sun.jpg");
  sun->Initialize(m_vertPos, m_vertNorm, m_vertText);

  planet = std::make_unique<Sphere>(
      "../assets/planetary_textures/2k_earth_daymap.jpg",
      "../assets/planetary_textures/2k_earth_daymap-n.jpg");
  planet->Initialize(m_vertPos, m_vertNorm, m_vertText);

  moon = std::make_unique<Sphere>("../assets/planetary_textures/2k_moon.jpg",
                                  "../assets/planetary_textures/2k_moon-n.jpg");
  moon->Initialize(m_vertPos, m_vertNorm, m_vertText);

  ship = std::make_unique<Mesh>("../assets/SpaceShip-1.obj",
                                "../assets/SpaceShip-1.png");
  ship->Initialize(m_vertPos, m_vertNorm, m_vertText);

  // enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(double dt) {
  glm::mat4 tmat, rmat, smat;
  tmat = glm::mat4(1);
  rmat = glm::rotate(glm::mat4(1), 0.5f * (float)dt, glm::vec3{0, 1, 0});
  smat = glm::scale(glm::vec3(1));
  sun->Update(tmat * rmat * smat);

  tmat = glm::translate(glm::mat4(1),
                        glm::vec3(0, cos(2 * dt) * 1.5, sin(2 * dt) * 1.5));
  rmat = glm::rotate(glm::mat4(1), 2 * (float)dt, glm::vec3{1, 0, 0});
  smat = glm::scale(glm::vec3(0.01f));
  ship->Update(tmat * rmat * smat);

  tmat = glm::translate(glm::mat4(1),
                        glm::vec3(cos(dt * 0.2) * 4, 0, sin(dt * 0.2) * 4));
  rmat = glm::rotate(glm::mat4(1), 3 * (float)dt, glm::vec3{0, 1, 0});
  smat = glm::scale(glm::vec3(0.5f));
  planet->Update(tmat * rmat * smat);

  tmat = glm::translate(
      glm::mat4(1),
      glm::vec3(-cos(2 * dt) * 2, sin(dt) * 1.0 / 2.0, -sin(2 * dt) * 2));
  rmat = glm::rotate(glm::mat4(1), 3 * (float)dt, glm::vec3{0, 1, 0});
  smat = glm::scale(glm::vec3(0.3f));
  moon->Update(tmat * rmat * smat);
}

void Graphics::Render() {
  // clear the screen
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // enable the skybox shader
  skybox->render(m_camera->GetProjection(), m_camera->GetView());

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetView()));
  glUniform3fv(view_pos_loc, 1, glm::value_ptr(m_camera->get_position()));

  std::stack<glm::mat4> stack;
  stack.push(glm::mat4(1.0f));

  stack.push(stack.top() * sun->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  sun->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
              light_pos_loc, light_color_loc, has_normal_map_loc,
              material_ambient_loc, material_specular_loc, material_diffuse_loc,
              material_shininess_loc, is_emissive_loc, true);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * ship->GetModel()));
  ship->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
               light_pos_loc, light_color_loc, has_normal_map_loc,
               material_ambient_loc, material_specular_loc,
               material_diffuse_loc, material_shininess_loc, is_emissive_loc,
               false);

  stack.push(stack.top() * planet->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  planet->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
                 light_pos_loc, light_color_loc, has_normal_map_loc,
                 material_ambient_loc, material_specular_loc,
                 material_diffuse_loc, material_shininess_loc, is_emissive_loc,
                 false);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * moon->GetModel()));
  moon->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc, m_sampler2Loc,
               light_pos_loc, light_color_loc, has_normal_map_loc,
               material_ambient_loc, material_specular_loc,
               material_diffuse_loc, material_shininess_loc, is_emissive_loc,
               false);

  stack.pop();
  stack.pop();
  stack.pop();

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
