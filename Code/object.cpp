#include "object.h"
#include "graphics_headers.h"

void Object::Initialize(GLint posAttribLoc, GLint vertAttribLoc,
                        GLint textAttribLoc) {
  // set up your VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // setting the VBO
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0],
               GL_STATIC_DRAW);
  glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(vertAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glVertexAttribPointer(textAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texture));

  // setting the Index VBO
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(),
               &Indices[0], GL_STATIC_DRAW);

  // set a random rotation angle between 0 and 2*pi
  angle = glm::linearRand(0.f, 2 * 3.1415f);

  // set x, y, z randomly between -5.-5.
  float tvec1 = glm::linearRand(-5.f, 5.f);
  float tvec2 = glm::linearRand(-5.f, 5.f);
  float tvec3 = glm::linearRand(-2.f, 2.f);

  // apply translation to identify matrix, assign to model
  model = glm::translate(glm::mat4(1.0f), glm::vec3(tvec1, tvec2, tvec3));

  // oriented randomly
  glm::vec3 axis = glm::normalize(glm::vec3(glm::linearRand(-1.f, 1.f),
                                            glm::linearRand(-1.f, 1.f),
                                            glm::linearRand(-1.f, 1.f)));

  // apply rotation
  model *= glm::rotate(glm::mat4(1.0f), angle, axis);
}

Object::~Object() {
  // clear Vertices and Indices vectors
  Vertices.clear();
  Indices.clear();
}

// update model matrix
void Object::Update(glm::mat4 model_matrix) { this->model = model_matrix; }

glm::mat4 Object::GetModel() {
  // return model matrix
  return model;
}

void Object::Render(GLint posAttribLoc, GLint vertAttribLoc,
                    GLint textAttribLoc, GLint samplerAttribloc,
                    GLint light_pos_loc, GLint light_color_loc,
                    GLint is_emissive_loc, bool is_emissive) {
  // bind VAO
  glBindVertexArray(vao);

  // bind VBO(s)
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  // enable the vertex attribute arrays
  // this is the poistion attrib in the vertex shader
  glEnableVertexAttribArray(posAttribLoc);
  glEnableVertexAttribArray(vertAttribLoc);
  glEnableVertexAttribArray(textAttribLoc);

  glUniform1i(samplerAttribloc, 0);
  glUniform1i(is_emissive_loc, is_emissive);
  glUniform3fv(light_pos_loc, 1, glm::value_ptr(glm::vec3(0, 0, 0)));
  glUniform3fv(light_color_loc, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texture->get_texture_id());

  // draw call to OpenGL
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  // disable the vertex attributes
  glDisableVertexAttribArray(posAttribLoc);
  glDisableVertexAttribArray(vertAttribLoc);
  glDisableVertexAttribArray(textAttribLoc);

  // unbind VBO(s) and ElementBuffer(s)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
