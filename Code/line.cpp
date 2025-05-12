#include "line.h"
#include "graphics_headers.h"

Line::Line(size_t max_trail) : max_trail(max_trail) {
  this->initialize();
  this->initialize_shaders();
}
Line::~Line() {}

void Line::add_position(glm::vec3 pos) {
  if (planet_trail.size() > max_trail) {
    planet_trail.erase(planet_trail.begin());
  }

  planet_trail.push_back(pos);
}

void Line::initialize() {
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->VB);
}

void Line::render(glm::mat4 projection, glm::mat4 view) {
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->VB);

  glBufferData(GL_ARRAY_BUFFER, this->planet_trail.size() * sizeof(glm::vec3),
               planet_trail.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

  glEnableVertexAttribArray(0);

  shader->Enable();
  glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
                     glm::value_ptr(projection));
  glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
                     glm::value_ptr(view));

  glDrawArrays(GL_LINE_STRIP, 0, this->planet_trail.size());
}

void Line::initialize_shaders() {
  this->shader = std::make_unique<Shader>();
  if (!this->shader->Initialize()) {
    std::cerr << "line shader failed to initialize" << std::endl;
    return;
  }

  std::string v = "#version 410\n \
									 layout (location = 0) in vec3 aPos;\
									 uniform mat4 projection;\
									 uniform mat4 view;\
									 void main() {\
										 gl_Position = projection * view * vec4(aPos, 1.0);\
									 }\
									";
  if (!this->shader->AttachShader(v, GL_VERTEX_SHADER)) {
    std::cerr << "Skybox vertex shader failed to initialize" << std::endl;
    return;
  }

  std::string f = "#version 410\n \
									 out vec4 frag_color;\
									 vec3 trailColor = vec3(1.0, 1.0, 1.0);\
									 void main() {\
										 frag_color = vec4(trailColor, 0.2);\
									}\
                  ";
  this->shader->AttachShader(f, GL_FRAGMENT_SHADER);

  if (!this->shader->Finalize()) {
    std::cerr << "line shader failed to finalize" << std::endl;
    return;
  }
}
