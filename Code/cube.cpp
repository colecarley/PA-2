#include "cube.h"

Cube::Cube() { createVertices(); }

void Cube::createVertices() {
  Vertices = {{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
              {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
              {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
              {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
              {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
              {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
              {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
              {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}};

  Indices = {1, 2, 3, 7, 6, 5, 0, 4, 5, 1, 5, 6, 6, 7, 3, 0, 3, 7,
             0, 1, 3, 4, 7, 5, 1, 0, 5, 2, 1, 6, 2, 6, 3, 4, 0, 7};
}

void Cube::Initialize(GLint posAttribLoc, GLint colAttribLoc) {
  // Set up your VOA
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // setting the Vertex VBO
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0],
               GL_STATIC_DRAW);
  glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));

  // Setting the Index VBO
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(),
               &Indices[0], GL_STATIC_DRAW);

  // Computing the model matrix
  // Model orientation

  // start the cube at a random angle
  srand(time(0));
  angle = 3.14159f * rand();

  float tvec1 = glm::linearRand(-5.f, 5.f);
  float tvec2 = glm::linearRand(-5.f, 5.f);
  float tvec3 = glm::linearRand(-2.f, 2.f);

  model = glm::translate(glm::mat4(1.0f), glm::vec3(tvec1, tvec2, tvec3));
  model *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0.0f, 1.0f));

  // set the initial angle to 0, so there is no rotation
  angle = 0;
}

Cube::~Cube() {
  Vertices.clear();
  Indices.clear();
}

void Cube::Update(unsigned int dt) {
  // translate the model
  model = glm::translate(model, m_speed);
  // rotate the model
  model = glm::rotate(model, angle, glm::vec3(0, 0.0f, 1.0f));
}

glm::mat4 Cube::GetModel() { return model; }

void Cube::Render(GLint posAttribLoc, GLint colAttribLoc) {
  // Bind VAO
  glBindVertexArray(vao);

  // Bind VBO(s)
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  // enable the vertex attribute arrays
  // this is the poistion attrib in the vertex shader
  glEnableVertexAttribArray(posAttribLoc);
  // this is the color attribe in the vertex shader
  glEnableVertexAttribArray(colAttribLoc);

  // Draw call to OpenGL
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  // disable the vertex attributes
  glDisableVertexAttribArray(posAttribLoc);
  glDisableVertexAttribArray(colAttribLoc);

  // unbind VBO(s) and ElementBuffer(s)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
