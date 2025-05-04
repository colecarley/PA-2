#include "graphics.h"
#include <memory>
#include <stack>
#include "graphics_headers.h"
#include "mesh.h"
#include "sphere.h"

Graphics::Graphics() {}
Graphics::~Graphics() {}

bool Graphics::Initialize(int width, int height) {
  // Init Camera
  m_camera = std::make_unique<Camera>();
  if (!m_camera->Initialize(width, height)) {
    printf("Camera Failed to Initialize\n");
    return false;
  }

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
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
    printf("m_modelMatrix not found\n");
    return false;
  }

	m_samplerLoc = m_shader->GetUniformLocation("samp");
	if (m_samplerLoc == INVALID_UNIFORM_LOCATION) {
		std::cerr << "m_samplerLoc not found\n";
		return false;
	}

  // Find where vertex attributes are in shader
  m_vertPos = m_shader->GetAttribLocation("v_position");
  m_vertNorm = m_shader->GetAttribLocation("v_normal");
	m_vertText = m_shader->GetAttribLocation("v_texture");

  // Create the object
  sun = std::make_unique<Sphere>("/Users/colecarley/src/PA-2/assets/2k_sun.jpg");
  sun->Initialize(m_vertPos, m_vertNorm, m_vertText);

  planet = std::make_unique<Sphere>("/Users/colecarley/src/PA-2/assets/2k_earth_daymap.jpg");
  planet->Initialize(m_vertPos, m_vertNorm, m_vertText);

  moon = std::make_unique<Sphere>("/Users/colecarley/src/PA-2/assets/2k_moon.jpg");
  moon->Initialize(m_vertPos, m_vertNorm, m_vertText);

  ship =
      std::make_unique<Mesh>("/Users/colecarley/src/PA-2/assets/SpaceShip-1.obj", "/Users/colecarley/src/PA-2/assets/SpaceShip-1.png");
  ship->Initialize(m_vertPos, m_vertNorm, m_vertText);

  // enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(double dt, glm::vec3 speed) {
  glm::mat4 tmat, rmat, smat;
  ComputeTransforms(dt, {0, 0, 0}, {0, 0, 0}, {0.0f}, glm::vec3{0, 1, 0},
                    {2, 2, 2}, tmat, rmat, smat);

  sun->Update(tmat * rmat * smat);

  ComputeTransforms(dt, {1, 0, 1}, {6, 0, 6}, {1}, glm::vec3{0, 1, 0},
                    {0.5f, 0.5f, 0.5f}, tmat, rmat, smat);

  planet->Update(tmat * rmat * smat);

  ComputeTransforms(dt, {3, 0, 3}, {2, 0, 2}, {1}, glm::vec3{0, 1, 0},
                    {0.4f, 0.4f, 0.4f}, tmat, rmat, smat);

  moon->Update(tmat * rmat * smat);

  ComputeTransforms(dt, {3, 3, 3}, {2, 2, 2}, {3}, glm::vec3{0, 0, 1},
                    {0.1f, 0.1f, 0.1f}, tmat, rmat, smat);
  rmat = glm::rotate(rmat, glm::radians(-90.0f), glm::vec3{0, 0, 1});
  ship->Update(tmat * rmat * smat);
}

void Graphics::Render() {
  // clear the screen
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE,
                     glm::value_ptr(m_camera->GetView()));

  std::stack<glm::mat4> stack;
  stack.push(glm::mat4(1.0f));

  stack.push(stack.top() * sun->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  sun->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * ship->GetModel()));
  ship->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc);

  stack.push(stack.top() * planet->GetModel());
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(stack.top()));
  planet->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE,
                     glm::value_ptr(stack.top() * moon->GetModel()));
  moon->Render(m_vertPos, m_vertNorm, m_vertText, m_samplerLoc);

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

std::unique_ptr<Object> &Graphics::getInteractWith() { return sun; }

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

void Graphics::ComputeTransforms(double dt, std::vector<float> speed,
                                 std::vector<float> dist,
                                 std::vector<float> rotSpeed,
                                 glm::vec3 rotVector, std::vector<float> scale,
                                 glm::mat4 &tmat, glm::mat4 &rmat,
                                 glm::mat4 &smat) {
  tmat =
      glm::translate(glm::mat4(1.0f), glm::vec3(cos(speed[0] * dt) * dist[0],
                                                sin(speed[1] * dt) * dist[1],
                                                sin(speed[2] * dt) * dist[2]));

  rmat = glm::rotate(glm::mat4(1.0f), rotSpeed[0] * (float)dt, rotVector);
  smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
} 
